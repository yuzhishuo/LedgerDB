/*
 * @Author: Leo
 * @Date: 2022-08-07 16:19:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-17 02:18:43
 */
#include "RaftServer.h"
#include <array>
#include <memory>
#include <string>
#include <utility/Enum.h>
#include <vector>

#include <stdlib.h>
namespace yuzhi::raft_engine
{
static thread_local RaftServerInterface *sv = nullptr;

void __peer_alloc_cb(uv_handle_t *, size_t size, uv_buf_t *buf);
void __peer_read_cb(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf);

void __periodic(uv_timer_t *handle);

size_t __peer_msg_serialize(tpl_node *tn, uv_buf_t *buf, char *data);
void __peer_msg_send(uv_stream_t *s, tpl_node *tn, uv_buf_t *buf, char *data);

void __send_handshake(RaftServerInterface *sv, peer_connection_t *conn);
int __send_handshake_response(peer_connection_t *conn, handshake_state_e success, raft_node_t *leader, int http_port,
                              int node_id);

peer_connection_t *__new_connection(uv_loop_t *peer_loop, peer_connection_t **conns, void *user_data);
void __delete_connection(RaftServerInterface *sv, peer_connection_t *conn);

peer_connection_t *__find_connection(RaftServerInterface *sv, const char *host, int raft_port);

void __load_commit_log(raft_server_t *sv);
void __load_persistent_state(raft_server_t *sv);

/* net init */
void __start_peer_socket(const char *host, int port, uv_loop_t *peer_loop, uv_tcp_t *listen, void *user_data);

/** Connect to raft peer */
void __connect_to_peer(peer_connection_t *conn);
void __connection_set_peer(peer_connection_t *conn, char *host, int port);
void __connect_to_peer_at_host(peer_connection_t *conn, char *host, int port);

// TODO: remove http_port , add specialization type for share
int __append_cfg_change(RaftServerInterface *sv, raft_logtype_e change_type, const char *host, uint16_t raft_port,
                        uint16_t http_port, uint16_t id);

int __offer_cfg_change(raft_server_t *raft, uv_loop_t *peer_loop, void *data, raft_logtype_e change_type)
{
  auto change = static_cast<entry_cfg_change_t *>(data);
  auto conn = __find_connection(sv, change->host, change->raft_port);

  /* Node is being removed */
  if (RAFT_LOGTYPE_REMOVE_NODE == change_type)
  {
    raft_remove_node(raft, raft_get_node(raft, change->node_id));
    if (conn)
      conn->node = nullptr;
    /* __delete_connection(sv, conn); */
    return 0;
  }

  /* Node is being added */
  if (!conn)
  {
    conn = __new_connection(peer_loop, &sv->conns, sv);
    __connection_set_peer(conn, change->host, change->raft_port);
  }
  conn->http_port = change->http_port;

  int is_self = change->node_id == sv->node_id;
  switch (change_type)
  {
  case RAFT_LOGTYPE_ADD_NONVOTING_NODE:
    conn->node = raft_add_non_voting_node(raft, conn, change->node_id, is_self);
    break;
  case RAFT_LOGTYPE_ADD_NODE:
    conn->node = raft_add_node(raft, conn, change->node_id, is_self);
    break;
  default:
    assert(0);
  }

  raft_node_set_udata(conn->node, conn);
  return 0;
}

/** Initiate connection if we are disconnected */
int __connect_if_needed(peer_connection_t *conn);

int __send_leave_response(peer_connection_t *conn);

#pragma region raft call define

int __raft_send_appendentries(raft_server_t *raft, void *user_data, raft_node_t *node, msg_appendentries_t *m);
int __raft_persist_vote(raft_server_t *raft, void *udata, raft_node_id_t vote);

int __raft_persist_term(raft_server_t *raft, void *user_data, raft_term_t term, raft_node_id_t vote);

int __raft_applylog(raft_server_t *raft, void *user_data, raft_entry_t *entry, raft_index_t entry_idx);

int __raft_logentry_offer(raft_server_t *raft, void *udata, raft_entry_t *ety, raft_index_t entry_idx);

int __raft_logentry_poll(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx);
int __raft_logentry_pop(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx);

#pragma endregion

bool RaftServerInterfaceInit(RaftServerInterface *raftServiceIniterface, void *user_data)
{
  if (raftServiceIniterface == nullptr)
    return false;

  raftServiceIniterface->raft = raft_new();
  raft_set_election_timeout(raftServiceIniterface->raft, 2000);

  uv_loop_init(&raftServiceIniterface->peer_loop);
  uv_mutex_init(&raftServiceIniterface->raft_lock);
  uv_cond_init(&raftServiceIniterface->appendentries_received);
  uv_timer_init(&raftServiceIniterface->peer_loop, &raftServiceIniterface->periodic_req);

  raftServiceIniterface->node_id = random() % (1 << utility::workerIdBits);
  assert(raftServiceIniterface->node_id >= 0 and raftServiceIniterface->node_id < 32);
  SPDLOG_DEBUG("Raft server node id is {}", raftServiceIniterface->node_id);
  raftServiceIniterface->user_data = user_data;
}

void RaftServerInterfaceDestroy(RaftServerInterface *raftServiceIniterface)
{
  uv_mutex_destroy(&raftServiceIniterface->raft_lock);
  uv_cond_destroy(&raftServiceIniterface->appendentries_received);
  raft_free(raftServiceIniterface->raft);
  raftServiceIniterface->raft = nullptr;
}

void RaftServerInterfaceRun(RaftServerInterface *raftServiceIniterface, IdleState state, int raft_port)
{
  if (state == IdleState::JOIN || state == IdleState::START)
  {
    __start_peer_socket("127.0.0.1", raft_port, &raftServiceIniterface->peer_loop, &raftServiceIniterface->peer_listen,
                        raftServiceIniterface);

    if (state == IdleState::START)
    {
      SPDLOG_DEBUG("Raft server starting");
      raft_become_leader(raftServiceIniterface->raft);
      // if (auto e = __append_cfg_change(raftServiceIniterface, RAFT_LOGTYPE_ADD_NODE,
      // raftServiceIniterface->opts.host, raft_port,
      //                                  /* http_port */ -1, raftServiceIniterface->node_id);
      //     e != 0)
      // {
      //   SPDLOG_DEBUG("Failed to append cfg change");
      // }
    }
    else
    {
      SPDLOG_DEBUG("Raft server joining");
      auto conn =
          __new_connection(&raftServiceIniterface->peer_loop, &raftServiceIniterface->conns, raftServiceIniterface);
      if (raftServiceIniterface->opts.cluster_address.has_value())
      {
        auto &addr = raftServiceIniterface->opts.cluster_address.value();

        __connect_to_peer_at_host(conn, addr.host, addr.port);
      }
    }
  }
  else
  {
    __start_peer_socket("127.0.0.1", raft_port, &raftServiceIniterface->peer_loop, &raftServiceIniterface->peer_listen,
                        raftServiceIniterface);
    __load_commit_log(raftServiceIniterface->raft);
    __load_persistent_state(raftServiceIniterface->raft);
    if (auto node_num = raft_get_num_nodes(sv->raft); 1 == node_num)
    {
      raft_become_leader(raftServiceIniterface->raft);
    }
    else
    {
      for (int i = 0; i < raft_get_num_nodes(raftServiceIniterface->raft); i++)
      {
        if (auto node = raft_get_node_from_idx(raftServiceIniterface->raft, i);
            raft_node_get_id(node) != raftServiceIniterface->node_id)
        {
          auto conn = static_cast<peer_connection_t *>(raft_node_get_udata(node));
          __connect_to_peer(conn);
        }
      }
    }
  }
  uv_run(&raftServiceIniterface->peer_loop, UV_RUN_DEFAULT);
  SPDLOG_INFO("Raft server started");
}

RaftServer::RaftServer(rocksdb::TransactionDB *db, int raft_port, int engine_port, address_t cluster_address)
    : RaftServer(db, raft_port, engine_port, IdleState::JOIN)
{
  this->interface_.opts.cluster_address = cluster_address;
}

RaftServer::RaftServer(rocksdb::TransactionDB *db, int raft_port, int engine_port, IdleState state) : generator_{db}
{
  SPDLOG_DEBUG("Raft server constructor");
  SPDLOG_DEBUG("Raft server idle state is {}", magic_enum::enum_name(state));
  RaftServerInterface *raftServiceIniterface = std::addressof(interface_);

  raftServiceIniterface->opts.host = "127.0.0.1";
  raftServiceIniterface->opts.raft_port = raft_port;
  raftServiceIniterface->opts.http_port = engine_port;
  raftServiceIniterface->opts.idle_state = state;

  auto connection_user_data = this;
  auto raft_user_data = this;
  // init successfully
  RaftServerInterfaceInit(raftServiceIniterface, raft_user_data);
  raft_add_node(raftServiceIniterface->raft, connection_user_data, raftServiceIniterface->node_id, true);

  if (sv != nullptr)
  {
    SPDLOG_ERROR("raft server already initialized");
    exit(1);
  }

  // each thread own a raft server_interface obejct
  sv = raftServiceIniterface;

  callback_init(raftServiceIniterface);
}

void RaftServer::start()
{
  RaftServerInterface *raftServiceIniterface = std::addressof(interface_);
  timer_init(raftServiceIniterface);
  RaftServerInterfaceRun(raftServiceIniterface, raftServiceIniterface->opts.idle_state,
                         raftServiceIniterface->opts.raft_port);
}

RaftServer::~RaftServer() { RaftServerInterfaceDestroy(&interface_); }

void RaftServer::timer_init(RaftServerInterface *raftServiceIniterface) const noexcept
{
  raftServiceIniterface->periodic_req.data = raftServiceIniterface->raft;
  uv_timer_start(&raftServiceIniterface->periodic_req, __periodic, 0, 1000);
}

#pragma region raft call define
int __raft_send_requestvote(raft_server_t *raft, void *udata, raft_node_t *node, msg_requestvote_t *m);
int __raft_send_appendentries(raft_server_t *raft, void *user_data, raft_node_t *node, msg_appendentries_t *m);
int __raft_applylog(raft_server_t *raft, void *user_data, raft_entry_t *entry, raft_index_t entry_idx);
int __raft_persist_vote(raft_server_t *raft, void *udata, raft_node_id_t vote);
int __raft_persist_term(raft_server_t *raft, void *user_data, raft_term_t term, raft_node_id_t vote);
int __raft_logentry_offer(raft_server_t *raft, void *udata, raft_entry_t *ety, raft_index_t entry_idx);
int __raft_logentry_poll(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx);
int __raft_logentry_pop(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx);
raft_node_id_t __raft_log_get_node_id(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx);
/** Non-voting node now has enough logs to be able to vote.
 * Append a finalization cfg log entry. */
int __raft_node_has_sufficient_logs(raft_server_t *raft, void *user_data, raft_node_t *node);
void __raft_log(raft_server_t *raft, raft_node_t *node, void *udata, const char *buf);
int __raft_send_snapshot(raft_server_t *raft, void *user_data, raft_node_t *node);
#pragma endregion raft call define

void RaftServer::callback_init(RaftServerInterface *raftServiceIniterface) noexcept
{
  SPDLOG_DEBUG("Raft server callback init");
  // shadows variable in outer scope
  auto sv = raftServiceIniterface;
  sv->raft_callbacks.send_requestvote = __raft_send_requestvote;
  sv->raft_callbacks.send_appendentries = __raft_send_appendentries;
  sv->raft_callbacks.applylog = __raft_applylog;
  sv->raft_callbacks.persist_vote = __raft_persist_vote;
  sv->raft_callbacks.persist_term = __raft_persist_term;
  sv->raft_callbacks.log_offer = __raft_logentry_offer;
  sv->raft_callbacks.log_poll = __raft_logentry_poll;
  sv->raft_callbacks.log_pop = __raft_logentry_pop;
  sv->raft_callbacks.node_has_sufficient_logs = __raft_node_has_sufficient_logs;
  sv->raft_callbacks.log = __raft_log;
  sv->raft_callbacks.log_get_node_id = __raft_log_get_node_id;
  sv->raft_callbacks.send_snapshot = __raft_send_snapshot;
  raft_set_callbacks(interface_.raft, &sv->raft_callbacks, this);
}

#pragma region raft call define

// FIXME: this function
int __raft_send_requestvote([[maybe_unused]] raft_server_t *raft, [[maybe_unused]] void *udata, raft_node_t *node,
                            msg_requestvote_t *m)
{
  SPDLOG_INFO("raft callback: __raft_send_requestvote");
  auto conn = static_cast<peer_connection_t *>(raft_node_get_udata(node));

  SPDLOG_INFO("__raft_send_requestvote: {}:{}", inet_ntoa(conn->addr.sin_addr), conn->addr.sin_port);

  uv_buf_t bufs;
  std::array<char, RAFT_BUFLEN> buf;
  msg_t msg = {.type = MSG_REQUESTVOTE, .rv = *m};
  __peer_msg_serialize(tpl_map("S(I$(IIII))", &msg), &bufs, buf.data());
  if (auto e = uv_try_write(conn->stream, &bufs, 1); e < 0)
  {
    SPDLOG_ERROR("Failed to send requestvote: {}", uv_strerror(e));
    uv_fatal(e);
  }
  return 0;
}

// FIXME: this function
int __raft_send_appendentries([[maybe_unused]] raft_server_t *raft, void *user_data, raft_node_t *node,
                              msg_appendentries_t *m)
{
  SPDLOG_INFO("raft callback: __raft_send_appendentries");

  auto conn = static_cast<peer_connection_t *>(raft_node_get_udata(node));
  SPDLOG_INFO("Sending appendentries to {}:{}", inet_ntoa(conn->addr.sin_addr), conn->addr.sin_port);
  if (auto e = __connect_if_needed(conn); - 1 == e)
  {
    SPDLOG_INFO("reconnect {}:{}", inet_ntoa(conn->addr.sin_addr), conn->addr.sin_port);
    return 0;
  }

  std::array<uv_buf_t, 3> bufs{};
  std::array<char, RAFT_BUFLEN> buf;

  char *ptr = buf.data();
  msg_t msg = {};
  msg.type = MSG_APPENDENTRIES;
  msg.ae.term = m->term;
  msg.ae.prev_log_idx = m->prev_log_idx;
  msg.ae.prev_log_term = m->prev_log_term;
  msg.ae.leader_commit = m->leader_commit;
  msg.ae.n_entries = m->n_entries;
  ptr += __peer_msg_serialize(tpl_map("S(I$(IIIII))", &msg), bufs.data(), ptr);

  /* appendentries with payload */
  if (0 < m->n_entries)
  {
    SPDLOG_INFO("appendentries with payload");
    tpl_bin tb;
    tb.sz = m->entries[0].data.len;
    tb.addr = m->entries[0].data.buf;

    /* list of entries */
    tpl_node *tn = tpl_map("IIIB", &m->entries[0].id, &m->entries[0].term, &m->entries[0].type, &tb);
    size_t sz;
    tpl_pack(tn, 0);
    tpl_dump(tn, TPL_GETSIZE, &sz);
    if (auto e = tpl_dump(tn, TPL_MEM | TPL_PREALLOCD, ptr, RAFT_BUFLEN); 0 != e)
    {
      SPDLOG_ERROR("Failed to pack appendentries");
      return -1;
    }
    bufs[1].len = sz;
    bufs[1].base = ptr;
    if (auto e = uv_try_write(conn->stream, bufs.data(), 2); e < 0)
    {
      SPDLOG_ERROR("Failed to send appendentries: {}", uv_strerror(e));
      uv_fatal(e)
    }
    tpl_free(tn);
  }
  else
  {
    SPDLOG_INFO("keepalive appendentries");
    /* keep alive appendentries only */

    if (auto e = uv_try_write(conn->stream, bufs.data(), 1); e < 0)
    {
      SPDLOG_ERROR("Failed to send appendentries: {}", uv_strerror(e));
      uv_fatal(e)
    }
  }

  return 0;
}

int RaftServer::sync(const yuzhi::IMonostate<raft::Entry> &mono)
{
  auto RaftServerInterface = std::addressof(interface_);
  auto raft = RaftServerInterface->raft;

  if (raft_node_t *leader = raft_get_current_leader_node(raft); !leader)
  {
    SPDLOG_ERROR("Couldn't push leader_conn");
    return -1;
  }
  else if (raft_node_get_id(leader) != RaftServerInterface->node_id)
  {
    auto leader_conn = raft_node_get_udata(leader);
    // TODO: send push to leader
    SPDLOG_INFO("push to leader");
    return -1;
  }

  auto [res, err] = mono.serialize();

  msg_entry_t entry = {};
  entry.id = generator_.getUnique();
  if (!err.has_value())
  {
    entry.data.buf = res.data();
    entry.data.len = res.size();
  }
  else
  {
    SPDLOG_ERROR("Failed to serialize leader data: {}", err.value().message());
    return -1;
  }
  int e;

  uv_mutex_lock(&RaftServerInterface->raft_lock);

  msg_entry_response_t r;

  if (e = raft_recv_entry(RaftServerInterface->raft, &entry, &r); 0 != e)
  {
    SPDLOG_ERROR("Failed to recv entry");
  }

  /* block until the entry is committed */
  bool done = false;
  int tries = 0;
  do
  {
    if (3 < tries)
    {
      printf("ERROR: failed to commit entry\n");
      uv_mutex_unlock(&RaftServerInterface->raft_lock);
      return -1;
    }

    uv_cond_wait(&RaftServerInterface->appendentries_received, &RaftServerInterface->raft_lock);
    tries += 1;
    switch (e = raft_msg_entry_response_committed(RaftServerInterface->raft, &r); e)
    {
    case 0:
      /* not committed yet */
      break;
    case 1:
      done = true;
      uv_mutex_unlock(&RaftServerInterface->raft_lock);
      break;
    case -1:
      uv_mutex_unlock(&RaftServerInterface->raft_lock);
      return -1;
    default:
      uv_mutex_unlock(&RaftServerInterface->raft_lock);
      return -1;
    }
  } while (!done);

  // could not arrived here yet
  uv_mutex_unlock(&RaftServerInterface->raft_lock);
  return -1;
}

bool RaftServer::isLeader() const
{
  auto RaftServerInterface = std::addressof(interface_);
  auto raft = RaftServerInterface->raft;
  return raft_is_leader(raft);
}

// FIXME: this function
int __raft_applylog(raft_server_t *raft, [[maybe_unused]] void *user_data, raft_entry_t *entry, raft_index_t entry_idx)
{

  SPDLOG_INFO("raft callback: Applying log entry {}", entry_idx);
  std::vector<std::pair<std::string, std::string>> entries;
  /* Check if it's a configuration change */
  if (raft_entry_is_cfg_change(entry))
  {
    auto change = static_cast<entry_cfg_change_t *>(entry->data.buf);
    if (RAFT_LOGTYPE_REMOVE_NODE != entry->type || !raft_is_leader(raft))
      goto final;
    auto conn = __find_connection(sv, change->host, change->raft_port);
    __send_leave_response(conn);
    goto final;
  }

  using namespace std;
  entries.emplace_back("commit_idx"s + to_string(raft_get_commit_idx(raft)),
                       string((char *)entry->data.buf, entry->data.len));
final:

  entries.emplace_back("commit_idx"s, to_string(raft_get_commit_idx(raft)));

  // TODO: throw up
  SPDLOG_ERROR("UNDINISH");
  return 0;
}

// FIXME: this function
int __raft_persist_vote(raft_server_t *raft, void *udata, raft_node_id_t vote)
{
  SPDLOG_INFO("raft callback: __raft_persist_vote");
  return 0;
}

// FIXME: This function
int __raft_persist_term(raft_server_t *raft, void *user_data, raft_term_t term, raft_node_id_t vote)
{
  SPDLOG_INFO("raft callback: __raft_persist_term");
  return 0;
}

// FIXME: This function
int __raft_logentry_offer(raft_server_t *raft, void *udata, raft_entry_t *ety, raft_index_t entry_idx)
{
  SPDLOG_INFO("raft callback: __raft_logentry_offer");
  auto raftServer = static_cast<RaftServer *>(udata);
  raftServer->sync_event_.crash(entry_idx);
  SPDLOG_DEBUG("raft callback: __raft_logentry_offer: {}", entry_idx);
  SPDLOG_DEBUG("__raft_logentry_offer: ety id: {}, term: {}, type: {}, msg: {}", ety->id, ety->term, ety->type,
               std::string((char *)ety->data.buf, ety->data.len));
  return 0;
}

int __raft_logentry_poll(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx)
{
  SPDLOG_INFO("__raft_logentry_poll");
  return -1;
}

// FIXME: This function
int __raft_logentry_pop(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx)
{
  SPDLOG_INFO("raft callback: __raft_logentry_pop");
  auto raftServer = static_cast<RaftServer *>(udata);
  // return raftServer->pop(entry, ety_idx);
  return -1;
}

int __raft_node_has_sufficient_logs(raft_server_t *raft, void *user_data, raft_node_t *node)
{
  SPDLOG_INFO("__raft_node_has_sufficient_logs");
  auto conn = static_cast<peer_connection_t *>(raft_node_get_udata(node));
  __append_cfg_change(sv, RAFT_LOGTYPE_ADD_NODE, inet_ntoa(conn->addr.sin_addr), conn->raft_port, conn->http_port,
                      raft_node_get_id(conn->node));
  return 0;
}

void __raft_log(raft_server_t *raft, raft_node_t *node, void *udata, const char *buf)
{
  SPDLOG_INFO("__raft_log");
  SPDLOG_DEBUG("__raft_log: buf {}", buf);
}

raft_node_id_t __raft_log_get_node_id(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx)
{
  SPDLOG_INFO("__raft_log_get_node_id");
  auto raftServer = static_cast<RaftServer *>(udata);
  // ERROR: node id should be the same as the one in the entry, like this
  // return  de(entry->data.buf, entry->data.len).node_id;
  return raftServer->interface_.node_id;
}

int __raft_send_snapshot(raft_server_t *raft, void *user_data, raft_node_t *node)
{

  SPDLOG_INFO("__raft_send_snapshot");
  raft_index_t snapshot_begin_index = raft_node_get_next_idx(node);
  raft_index_t snapshot_last_index = raft_get_snapshot_last_idx(raft);

  if (raft_index_t snapshot_diff = snapshot_last_index - snapshot_begin_index; snapshot_diff > 0)
  {
    SPDLOG_INFO("Snapshot lost from {} to {}", snapshot_begin_index, snapshot_last_index);
    SPDLOG_INFO("Sending snapshot to {}", raft_node_get_id(node));

    // FIXME: snapshot must be implemented
    // get_save_snapshot(snapshot_begin_index, snapshot_last_index);
    return 0;
  }
  return 0;
}

#pragma endregion raft call define

} // namespace yuzhi::raft_engine
