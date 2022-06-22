/*
 * @Author: Leo
 * @Date: 2022-02-03 16:06:57
 * @LastEditTime: 2022-03-11 14:26:23
 * @LastEditors: Leo
 * @Description:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/src/RaftService.cpp
 */

#include <chrono>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpServer.h>
#include <raft_engine/net/RaftService.hpp>
#include <raft_engine/net/interface_raft.h>
#include <rocksdb/db.h>
#include <utility/tpl.h>

// g++ bug: friend declaration for ‘__deserialize_and_handle_msg’ not found
namespace yuzhi::raft_engine::net {
using namespace std;
using namespace muduo;
using namespace muduo::net;

static RaftService *self = nullptr;

peer_connection_t *__new_connection();
void __connect_to_peer(peer_connection_t *conn);
void __connect_to_peer_at_host(peer_connection_t *conn, char *host, int port);
void __peer_msg_send(peer_connection_t *conn, tpl_node *tn, Buffer *buf,
                     char *data);
void __connection_set_peer(peer_connection_t *conn, char *host, int port);
peer_connection_t *__find_connection(const muduo::net::InetAddress &addr);
int __offer_cfg_change(raft_server_t *raft, const unsigned char *data,
                       raft_logtype_e change_type) {

  auto sv = self;
  auto change = (entry_cfg_change_t *)data;
  muduo::net::InetAddress addr(change->host, change->raft_port);
  peer_connection_t *conn = __find_connection(addr); // 找到对应的连接

  /* Node is being removed */
  if (RAFT_LOGTYPE_REMOVE_NODE == change_type) {
    raft_remove_node(raft,
                     raft_get_node((raft_server_t *)sv->raft, change->node_id));
    if (conn)
      conn->node = NULL;
    /* __delete_connection(sv, conn); */
    return 0;
  }

  /* Node is being added */
  if (!conn) {
    conn = __new_connection();
    __connection_set_peer(conn, change->host, change->raft_port);
  }
  conn->http_port = change->http_port;

  int is_self = change->node_id == sv->node_id;

  switch (change_type) {
  case RAFT_LOGTYPE_ADD_NONVOTING_NODE:
    conn->node = raft_add_non_voting_node(raft, conn, change->node_id, is_self);
    break;
  case RAFT_LOGTYPE_ADD_NODE:
    conn->node = raft_add_node(raft, conn, change->node_id, is_self);
    break;
  default:
    assert(0);
  }

  if (conn->node && !raft_is_leader(raft)) {
    raft_node_set_udata(conn->node, conn);
  } else if (!raft_is_leader(raft)) {
    SPDLOG_INFO("conn->node is null");
  }

  return 0;
}

void __send_leave(peer_connection_t *conn) {
  Buffer bufs[1];
  char buf[RAFT_BUFLEN];
  msg_t msg = {.type = MSG_LEAVE};
  auto fmt = string("S(I)");
  __peer_msg_send(conn, tpl_map(fmt.data(), &msg), bufs, buf);
}

int __send_leave_response(peer_connection_t *conn) {
  if (!conn) {
    spdlog::error("no connection??\n");
    return -1;
  }
  if (!conn->peer) {
    spdlog::error("no perr??\n");
    return -1;
  }

  Buffer bufs[1];
  char buf[RAFT_BUFLEN];
  msg_t msg = {.type = MSG_LEAVE_RESPONSE};
  string fmt = "S(I)";
  __peer_msg_send(conn, tpl_map(fmt.data(), &msg), bufs, buf);
  return 0;
}
static int __connect_if_needed(peer_connection_t *conn) {
  if (CONNECTED != conn->connection_status) {
    if (DISCONNECTED == conn->connection_status)
      __connect_to_peer(conn);
    return -1;
  }
  return 0;
}

/** Serialize a peer message using TPL
 * @param[out] bufs Muduo buffer to insert serialized message into
 * @param[out] buf Buffer to write serialized message into */
static size_t __peer_msg_serialize(tpl_node *tn, muduo::net::Buffer *buf,
                                   char *data) {
  size_t sz;
  tpl_pack(tn, 0);
  tpl_dump(tn, TPL_GETSIZE, &sz);
  tpl_dump(tn, TPL_MEM | TPL_PREALLOCD, data, RAFT_BUFLEN);
  tpl_free(tn);
  buf->append(data, sz);
  return sz;
}

void __peer_msg_send(peer_connection_t *conn, tpl_node *tn, Buffer *buf,
                     char *data) {
  __peer_msg_serialize(tn, buf, data);
  conn->peer->send(buf);
}

/** Deserialize a single log entry from appendentries message */
static void __deserialize_appendentries_payload(msg_entry_t *out,
                                                peer_connection_t *conn,
                                                void *img, size_t sz) {
  tpl_bin tb;
  tpl_node *tn = tpl_map(tpl_peek(TPL_MEM, img, sz), &out->id, &out->term,
                         &out->type, &tb);
  tpl_load(tn, TPL_MEM, img, sz);
  tpl_unpack(tn, 0);
  out->data.buf = tb.addr;
  out->data.len = tb.sz;
}

bool operator==(const muduo::net::InetAddress &left,
                const muduo::net::InetAddress &right) {
  return left.toIpPort() == right.toIpPort();
}

void __delete_connection(peer_connection_t *conn) {

  peer_connection_t *prev = NULL;
  if (self->conns == conn)
    self->conns = conn->next;
  else if (self->conns != conn) {
    for (prev = self->conns; prev->next != conn; prev = prev->next)
      ;
    prev->next = conn->next;
  } else
    assert(0);

  if (conn->node)
    raft_node_set_udata(conn->node, NULL);

  // TODO: make sure all resources are freed
  free(conn);
}

peer_connection_t *__find_connection(const muduo::net::InetAddress &addr) {
  peer_connection_t *conn;
  for (conn = self->conns; conn && conn->addr == addr; conn = conn->next)
    ;
  return conn;
}

void __send_handshake(peer_connection_t *conn) {
  Buffer bufs[1];
  char buf[RAFT_BUFLEN];
  msg_t msg = {};
  msg.type = MSG_HANDSHAKE;
  msg.hs.raft_port = self->raft_port;
  msg.hs.http_port = self->http_port;
  msg.hs.node_id = self->node_id;
  auto fmt = string("S(I$(IIII))");
  __peer_msg_send(conn, tpl_map(fmt.data(), &msg), bufs, buf);
}

int __send_handshake_response(int node_id, peer_connection_t *conn,
                              handshake_state_e success, raft_node_t *leader,
                              int16_t http_port) {
  SPDLOG_INFO("send handshake response");
  Buffer bufs[1];
  char buf[RAFT_BUFLEN];

  msg_t msg = {.type = MSG_HANDSHAKE_RESPONSE,
               .hsr = {
                   .success = success,
                   .leader_port = 0,
                   .node_id = node_id,
               }};

  /* allow the peer to redirect to the leader */
  if (leader) {
    if (auto leader_conn = (peer_connection_t *)raft_node_get_udata(leader);
        leader_conn) {
      msg.hsr.leader_port = leader_conn->raft_port;
      snprintf(msg.hsr.leader_host, IP_STR_LEN, "%s",
               leader_conn->addr.toIpPort().data());
    }
  }

  msg.hsr.http_port = http_port;
  string str = "S(I$(IIIIs))";
  __peer_msg_send(conn, tpl_map(str.data(), &msg), bufs, buf);
  return 0;
}

int __deserialize_and_handle_msg(void *img, size_t sz, void *data) {
  auto conn = (peer_connection_t *)data;
  msg_t m;
  int e;

  Buffer bufs[1];
  char buf[RAFT_BUFLEN];

  /* special case: handle appendentries payload */
  if (0 < conn->n_expected_entries) {
    msg_entry_t entry;

    __deserialize_appendentries_payload(&entry, conn, img, sz);

    conn->ae.ae.entries = &entry;
    msg_t msg = {.type = MSG_APPENDENTRIES_RESPONSE};
    e = raft_recv_appendentries((raft_server_t *)(self->raft), conn->node,
                                &conn->ae.ae, &msg.aer);

    /* send response */
    std::string fmt = "S(I$(IIII))";
    __peer_msg_send(conn, tpl_map(fmt.data(), &msg), bufs, buf);
    conn->n_expected_entries = 0;
    return 0;
  }

  /* deserialize message */
  tpl_node *tn = tpl_map(tpl_peek(TPL_MEM, img, sz), &m);
  tpl_load(tn, TPL_MEM, img, sz);
  tpl_unpack(tn, 0);

  switch (m.type) {
  case MSG_HANDSHAKE: {
    peer_connection_t *nconn = __find_connection(conn->addr);
    if (nconn && conn != nconn)
      __delete_connection(nconn);

    conn->connection_status = CONNECTED;
    conn->http_port = m.hs.http_port;
    conn->raft_port = m.hs.raft_port;

    raft_node_t *leader =
        raft_get_current_leader_node((raft_server_t *)self->raft);

    /* Is this peer in our configuration already? */
    raft_node_t *node =
        raft_get_node((raft_server_t *)self->raft, m.hs.node_id);
    if (node) {
      raft_node_set_udata(node, conn);
      conn->node = node;
    }

    if (!leader) {
      return __send_handshake_response(self->node_id, conn, HANDSHAKE_FAILURE,
                                       NULL, self->http_port);
    } else if (raft_node_get_id(leader) != self->node_id) {
      return __send_handshake_response(self->node_id, conn, HANDSHAKE_FAILURE,
                                       leader, self->http_port);
    } else if (node) {
      return __send_handshake_response(self->node_id, conn, HANDSHAKE_SUCCESS,
                                       NULL, self->http_port);
    } else {
      int e = __append_cfg_change(self, RAFT_LOGTYPE_ADD_NONVOTING_NODE,
                                  conn->addr.toIp().data(), m.hs.raft_port,
                                  m.hs.http_port, m.hs.node_id);
      if (0 != e)
        return __send_handshake_response(self->node_id, conn, HANDSHAKE_FAILURE,
                                         NULL, self->http_port);
      return __send_handshake_response(self->node_id, conn, HANDSHAKE_SUCCESS,
                                       NULL, self->http_port);
    }
  } break;
  case MSG_HANDSHAKE_RESPONSE:
    if (0 == m.hsr.success) {
      conn->http_port = m.hsr.http_port;

      /* We're being redirected to the leader */
      if (m.hsr.leader_port) {
        peer_connection_t *nconn = __find_connection(
            InetAddress{m.hsr.leader_host, m.hsr.leader_port});
        if (!nconn) {
          nconn = __new_connection();
          printf("Redirecting to %s:%d...\n", m.hsr.leader_host,
                 m.hsr.leader_port);
          __connect_to_peer_at_host(nconn, m.hsr.leader_host,
                                    m.hsr.leader_port);
        }
      }
    } else {
      printf("Connected to leader: %s:%d\n", conn->addr.toIp().data(),
             conn->raft_port);
      if (!conn->node)
        conn->node = raft_get_node((raft_server_t *)self->raft, m.hsr.node_id);
    }
    break;
  case MSG_LEAVE: {
    if (!conn->node) {
      printf("ERROR: no node\n");
      return 0;
    }
    int e = __append_cfg_change(self, RAFT_LOGTYPE_REMOVE_NODE,
                                conn->addr.toIp().data(), conn->raft_port,
                                conn->http_port, raft_node_get_id(conn->node));
    if (0 != e)
      printf("ERROR: Leave request failed\n");
  } break;
  case MSG_LEAVE_RESPONSE:
    // __drop_db(sv);
    printf("Shutdown complete. Quitting...\n");
    exit(0);
    break;
  case MSG_REQUESTVOTE: {
    msg_t msg = {.type = MSG_REQUESTVOTE_RESPONSE};
    e = raft_recv_requestvote((raft_server_t *)self->raft, conn->node, &m.rv,
                              &msg.rvr);
    string fmt = "S(I$(II))";
    __peer_msg_send(conn, tpl_map(fmt.data(), &msg), bufs, buf);
  } break;
  case MSG_REQUESTVOTE_RESPONSE:
    e = raft_recv_requestvote_response((raft_server_t *)self->raft, conn->node,
                                       &m.rvr);
    break;
  case MSG_APPENDENTRIES:
    /* special case: get ready to handle appendentries payload */
    if (0 < m.ae.n_entries) {
      conn->n_expected_entries = m.ae.n_entries;
      memcpy(&conn->ae, &m, sizeof(msg_t));
      return 0;
    }
    {
      /* this is a keep alive message */
      msg_t msg = {.type = MSG_APPENDENTRIES_RESPONSE};
      e = raft_recv_appendentries((raft_server_t *)self->raft, conn->node,
                                  &m.ae, &msg.aer);

      string fmt = "S(I$(IIII))";
      __peer_msg_send(conn, tpl_map(fmt.data(), &msg), bufs, buf);
    }
    break;
  case MSG_APPENDENTRIES_RESPONSE:
    e = raft_recv_appendentries_response((raft_server_t *)self->raft,
                                         conn->node, &m.aer);
    self->cond.notify_all();
    break;
  default:
    printf("unknown msg\n");
    exit(0);
  }
  return 0;
}

int __append_cfg_change(RaftService *sv, raft_logtype_e change_type, char *host,
                        int raft_port, int http_port, int node_id) {
  // FIXME: memory leak
  SPDLOG_INFO("Appending cfg change: {} {} {} {} {}", change_type, host,
              raft_port, http_port, node_id);
  entry_cfg_change_t *change = (entry_cfg_change_t *)calloc(1, sizeof(*change));
  change->raft_port = raft_port;
  change->http_port = http_port;
  change->node_id = node_id;
  strcpy(change->host, host);
  change->host[IP_STR_LEN - 1] = 0;

  msg_entry_t entry = {.id = rand(),
                       .type = change_type,
                       .data =
                           {
                               .buf = (void *)change,
                               .len = sizeof(*change),
                           }

  };
  msg_entry_response_t r;
  if (int e = raft_recv_entry((raft_server_t *)self->raft, &entry, &r);
      0 != e) {
    printf("ERROR: failed to append entry\n");
    return e;
  }
  return 0;
}

/** Raft callback for applying an entry to the finite state machine */
// TODO: import this from raft.h
int __raft_applylog(raft_server_t *raft, void *udata, raft_entry_t *ety,
                    raft_index_t entry_idx) {

  SPDLOG_INFO("Applying log entry {}", entry_idx);

  std::vector<std::pair<std::string, std::string>> kvs;

  /* Check if it's a configuration change */
  if (raft_entry_is_cfg_change(ety)) {
    auto change = (entry_cfg_change_t *)(((raft_entry_t *)ety)->data.buf);
    if (RAFT_LOGTYPE_REMOVE_NODE != ety->type ||
        !raft_is_leader((raft_server_t *)(self->raft)))
      goto commit;
    peer_connection_t *conn =
        __find_connection(InetAddress{change->host, change->raft_port});

    __send_leave_response(conn);
    goto commit;
  }

  kvs.push_back(
      std::make_pair(std::string((char *)ety->data.buf, ety->data.len), ""));

commit:

  kvs.push_back(std::make_pair(std::string("commit_idx"),
                               std::to_string(raft_get_commit_idx(raft))));

  self->persistenceStore.save(kvs);
  return 0;
}

/** Raft callback for saving voted_for field to disk.
 * This only returns when change has been made to disk. */
int __raft_persist_vote(raft_server_t *raft, void *udata, const int voted_for) {
  SPDLOG_INFO("Persisting vote for {}", voted_for);
  self->persistenceStore.save("voted_for", std::to_string(voted_for));
  return 0;
}

/** Raft callback for saving term field to disk.
 * This only returns when change has been made to disk. */
int __raft_persist_term(raft_server_t *raft, void *udata,
                        raft_term_t current_term, raft_node_id_t vote) {
  SPDLOG_INFO("Persisting term {}", current_term);
  self->persistenceStore.save("current_term", std::to_string(current_term));
  return 0;
}

/** Raft callback for appending an item to the log */
// TODO: import this from raft.c
int __raft_logentry_offer(raft_server_t *raft, void *udata, raft_entry_t *ety,
                          raft_index_t ety_idx) {
  SPDLOG_INFO("__raft_logentry_offer");

  if (raft_entry_is_cfg_change(ety))
    __offer_cfg_change(raft, (const unsigned char *)ety->data.buf,
                       (raft_logtype_e)ety->type);

  Buffer bufs[1];
  char buf[RAFT_BUFLEN];
  string fmt = "S(III)";
  __peer_msg_serialize(tpl_map(fmt.data(), ety), bufs, buf);

  /* 1. put metadata */
  std::string meta_key = "raft_log_index";
  std::string value = std::to_string(ety_idx);
  if (auto e = self->persistenceStore.update_key(meta_key, value); e) {
    SPDLOG_ERROR("Failed to update key {}, error message : {}", meta_key,
                 e->message());
    if (auto e = self->persistenceStore.save(meta_key, value); e) {
      SPDLOG_ERROR("__raft_logentry_offer put metadata error: {}",
                   e->message());
      return -1;
    }
  }
  /* 2. put entry */
  auto current_time = std::chrono::steady_clock::now().time_since_epoch();
  auto sec = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time);

  auto entry_key = to_string(sec.count()) + "-" + to_string(ety_idx);
  if (auto e = self->persistenceStore.save(entry_key, (char *)ety->data.buf);
      e) {
    SPDLOG_ERROR("__raft_logentry_offer put entry error: {}", e->message());
    return -1;
  }
  return 0;
}

/** Raft callback for removing the first entry from the log
 * @note this is provided to support log compaction in the future */
// TODO: unnecessary for now
int __raft_logentry_poll(raft_server_t *raft, void *udata, raft_entry_t *entry,
                         raft_index_t ety_idx) {
  SPDLOG_INFO("__raft_logentry_poll");
  return 0;
}

/** Raft callback for deleting the most recent entry from the log.
 * This happens when an invalid leader finds a valid leader and has to delete
 * superseded log entries. */
// TODO: temporary unnecessary function, 无效leader删除日志,保持一致性
int __raft_logentry_pop(raft_server_t *raft, void *udata, raft_entry_t *entry,
                        raft_index_t ety_idx) {
  SPDLOG_INFO("__raft_logentry_pop");
  return 0;
}

/** Non-voting node now has enough logs to be able to vote.
 * Append a finalization cfg log entry. */
int __raft_node_has_sufficient_logs(raft_server_t *raft, void *user_data,
                                    raft_node_t *node) {
  SPDLOG_INFO("__raft_node_has_sufficient_logs");
  peer_connection_t *conn = (peer_connection_t *)raft_node_get_udata(node);
  __append_cfg_change(self, RAFT_LOGTYPE_ADD_NODE, conn->addr.toIp().data(),
                      conn->raft_port, conn->http_port,
                      raft_node_get_id(conn->node));
  return -1;
}

/** Raft callback for displaying debugging information */
static void __raft_log(raft_server_t *raft, raft_node_t *node, void *udata,
                       const char *buf) {
  spdlog::debug("raft log : {}", buf);
}

peer_connection_t *__new_connection() {
  SPDLOG_INFO("__new_connection");
  peer_connection_t *conn =
      (peer_connection_t *)calloc(1, sizeof(peer_connection_t));
  conn->next = self->conns;
  self->conns = conn;
  return conn;
}
int __raft_logget_node_id(raft_server_t *raft, void *user_data,
                          raft_entry_t *entry, raft_index_t entry_idx) {
  return self->node_id;
}

void __on_connection_accepted_by_peer(peer_connection_t *data,
                                      const TcpConnectionPtr &conn) {
  spdlog::info("RaftService::__on_connection_accepted_by_peer");

  if (conn->connected()) {
    data->peer = conn;
    data->addr = conn->peerAddress();
    data->connection_status = CONNECTED;
    __send_handshake(data);
    conn->setContext(conn);
  }
}

void __connect_to_peer(peer_connection_t *conn) {
  assert(self);

  self->cls_.emplace_back(
      std::make_unique<TcpClient>(&self->loop_, conn->addr, "RaftClient"));
  auto &cl = self->cls_.back();
  conn->connection_status = CONNECTING;
  cl->setConnectionCallback(
      std::bind(&__on_connection_accepted_by_peer, conn, _1));
  cl->connect();
}

void __connection_set_peer(peer_connection_t *conn, char *host, int port) {
  conn->raft_port = port;
  SPDLOG_INFO("Connecting to {}:{}", host, conn->raft_port);
  muduo::net::InetAddress addr(host, port);
  conn->addr = addr;
}

void __connect_to_peer_at_host(peer_connection_t *conn, char *host, int port) {
  __connection_set_peer(conn, host, port);
  __connect_to_peer(conn);
}

static int __raft_send_appendentries(raft_server_t *raft, void *user_data,
                                     raft_node_t *node,
                                     msg_appendentries_t *m) {
  SPDLOG_INFO("__raft_send_appendentries");
  Buffer bufs[3];
  auto conn = (peer_connection_t *)raft_node_get_udata(node);

  if (auto e = __connect_if_needed(conn); - 1 == e)
    return 0;

  char buf[RAFT_BUFLEN], *ptr = buf;
  msg_t msg = {};
  msg.type = MSG_APPENDENTRIES;
  msg.ae.term = m->term;
  msg.ae.prev_log_idx = m->prev_log_idx;
  msg.ae.prev_log_term = m->prev_log_term;
  msg.ae.leader_commit = m->leader_commit;
  msg.ae.n_entries = m->n_entries;

  string fmt = "S(I$(IIIII))";
  ptr += __peer_msg_serialize(tpl_map(fmt.data(), &msg), bufs, ptr);

  /* appendentries with payload */
  if (0 < m->n_entries) {
    tpl_bin tb;

    tb.sz = m->entries[0].data.len;
    tb.addr = m->entries[0].data.buf;

    /* list of entries */
    string fmt = "IIIB";
    tpl_node *tn = tpl_map(fmt.data(), &m->entries[0].id, &m->entries[0].term,
                           &m->entries[0].type, &tb);
    size_t sz;
    tpl_pack(tn, 0);
    tpl_dump(tn, TPL_GETSIZE, &sz);
    auto e = tpl_dump(tn, TPL_MEM | TPL_PREALLOCD, ptr, RAFT_BUFLEN);
    assert(0 == e);
    bufs[1].append(ptr, sz); /* append serialized entry to buffer */
    conn->peer->send(&bufs[2]);
    tpl_free(tn);
  } else {
    /* keep alive appendentries only */
    conn->peer->send(&bufs[1]);
  }

  return 0;
}

/**
 * @brief
 *
 * @param raft
 * @param user_data
 * @param node
 * @param m
 * @return int
 * @note
 * TODO: important
 */
int __raft_send_requestvote(raft_server_t *raft, void *user_data,
                            raft_node_t *node, msg_requestvote_t *m) {
  SPDLOG_INFO("__raft_send_requestvote");

  peer_connection_t *conn = (peer_connection_t *)raft_node_get_udata(node);

  if (int e = __connect_if_needed(conn); e == -1)
    return 0;

  Buffer buf;
  char data[RAFT_BUFLEN];
  msg_t msg = {.type = MSG_REQUESTVOTE, .rv = *m};

  string fmt = "S(I$(IIII))";
  __peer_msg_send(conn, tpl_map(fmt.data(), &msg), &buf, data);
  return 0;
}

raft_cbs_t raft_funcs = {
    .send_requestvote = __raft_send_requestvote,     // need
    .send_appendentries = __raft_send_appendentries, // need
    .applylog = __raft_applylog,                     // need
    .persist_vote = __raft_persist_vote,             // need
    .persist_term = __raft_persist_term,             // need

    .log_offer = __raft_logentry_offer, // need
    .log_poll = __raft_logentry_poll,
    .log_pop = __raft_logentry_pop,
    .node_has_sufficient_logs = __raft_node_has_sufficient_logs,

    .log = __raft_log,
};

RaftService::RaftService() : persistenceStore("raft", "raft_store") {
  // unknow
  raft_funcs.log_get_node_id = __raft_logget_node_id;
  assert(!self);
  self = this;

  auto &config = yuzhi::Config::Instance();
  auto startable = config.get<bool>(this, "start");
  auto joinable = config.get<bool>(this, "join");
  auto host = config.get<std::string>(this, "server_address");
  auto raft_port = config.get<int>(this, "server_port");
  http_port = config.get<int>(this, "http_port");

  SPDLOG_INFO("RaftService::config is startable: {}, joinable: {}, host: {}, "
              "raft_port: {}, http_port: {}",
              startable, joinable, host, raft_port, http_port);

  raft = raft_new();

  if (startable || joinable) {
    node_id = config.get<int>(this, "node_id");
  } else {
    spdlog::error("Please start or join a cluster.");
    abort();
  }

  raft_set_callbacks((raft_server_t *)raft, &raft_funcs, this);
  auto node = raft_add_node((raft_server_t *)raft, NULL, node_id, 1);
  assert(node);

  if (startable || joinable) {

    uint16_t port = static_cast<uint16_t>(raft_port);
    InetAddress serverAddr(port);

    SPDLOG_INFO("bind message callback on Connection && Message");
    server_ = std::move(std::make_unique<muduo::net::TcpServer>(
        &loop_, serverAddr, "RaftServer"));
    server_->setConnectionCallback(
        bind(&RaftService::onConnection, this, std::placeholders::_1));
    server_->setMessageCallback(
        bind(&RaftService::onMessage, this, std::placeholders::_1,
             std::placeholders::_2, std::placeholders::_3));
    if (startable) {
      raft_become_leader((raft_server_t *)raft);
      /* We store membership configuration inside the Raft log.
       * This configuration change is going to be the initial membership
       * configuration (ie. original node) inside the Raft log. The
       * first configuration is for a cluster of 1 node. */
      __append_cfg_change(this, RAFT_LOGTYPE_ADD_NODE, host.data(), raft_port,
                          http_port, node_id);
    } else {
      // joinable， 会连接到集群 不确定是不是 leader
      peer_connection_t *conn = __new_connection();
      __connect_to_peer_at_host(conn, host.data(), port);
    }
  } else {
    spdlog::error("Please start or join a cluster.");
    abort();
  }

  constexpr auto PERIOD_MSEC = 1000;
  loop_.runEvery(PERIOD_MSEC,
                 std::bind(raft_periodic, (raft_server_t *)raft, PERIOD_MSEC));
  raft_set_election_timeout((raft_server_t *)raft, PERIOD_MSEC * 2);
}

void RaftService::onConnection(const TcpConnectionPtr &conn) {

  SPDLOG_INFO("{} -> {} is {}", conn->localAddress().toIpPort(),
              conn->peerAddress().toIpPort(),
              conn->connected() ? "UP" : "DOWN");

  if (conn->connected()) {
    peer_connection_t *conn1 = __new_connection();
    conn1->connection_status = CONNECTED;
    conn1->node = NULL;
    conn1->peer = conn;
    conn1->addr = conn->peerAddress();
    SPDLOG_INFO(
        "Connection established, assemble peer with coonection, addr: {}",
        conn1->addr.toIpPort());
    conn->setContext(conn1);
  }
}

/**
 * @brief RaftService::onConnection
 * @param conn
 * @details like __on_peer_connection
 */
void RaftService::onMessage(const muduo::net::TcpConnectionPtr &conn,
                            muduo::net::Buffer *buf,
                            muduo::Timestamp receiveTime) {
  if (!conn->connected()) {
    spdlog::error("Connection is not connected");
    return;
  }
  if (0 < buf->readableBytes()) {
    SPDLOG_INFO("raft onMessage timesatmp:{}", receiveTime.toString());
    auto conn1 = std::any_cast<peer_connection_t *>(conn->getContext());
    assert(conn1);
    auto bf = buf->retrieveAllAsString();
    std::lock_guard<std::mutex> lock(mutex_);
    tpl_gather(TPL_GATHER_MEM, bf.data(), bf.size(), &conn1->gt,
               __deserialize_and_handle_msg, conn1);
  }
}

std::optional<Error> RaftService::Save(const std::string &key,
                                       const std::string &value) {
  raft_node_t *leader = raft_get_current_leader_node((raft_server_t *)raft);

  if (!leader) {
    SPDLOG_ERROR("inter error, No leader node");
    return Error::UnLeader();
  }

  if (raft_node_get_id(leader) != node_id) {
    SPDLOG_ERROR("inter error, leader node is not me");
    return Error::Redirect();
  }

  auto en = key + "&yuzhi&" + value;
  msg_entry_t entry = {};
  entry.id = rand();
  entry.data.buf = (void *)en.data();
  entry.data.len = sizeof(en.size());

  SPDLOG_INFO("raft_append_entry, node_id: {}, entry.id: {}, data:{},"
              "entry.data.len: {} ",
              node_id, entry.id, key, entry.data.len);

  std::unique_lock<std::mutex> lk(mutex_, std::try_to_lock);
  SPDLOG_INFO("raft_append_entry will be lock"); // only test

  msg_entry_response_t r;
  if (auto e = raft_recv_entry((raft_server_t *)raft, &entry, &r); e != 0) {
    SPDLOG_ERROR("raft inter error, node_id : {} ", node_id);
    SPDLOG_ERROR("raft error code : {}", e);
    return Error::RaftError();
  }

  int done = 0, tries = 0;
  do {
    if (3 < tries) {
      SPDLOG_ERROR("tries 3s, failed to commit entry");
      return Error::RaftError();
    }

    // if  node num is 1, then commit immediately
    auto node_num = raft_get_num_voting_nodes((raft_server_t *)raft);
    if (1 < node_num) {
      SPDLOG_INFO("raft_append_entry will be condition lock"); // only test
      cond.wait(lk);
    }

    auto e = raft_msg_entry_response_committed((raft_server_t *)raft, &r);
    tries += 1;
    switch (e) {
    case 0:
      /* not committed yet */
      break;
    case 1:
      done = 1;
      lk.unlock();
      break;
    case -1:
      lk.unlock();
      SPDLOG_ERROR("raft inter error, node_id : {} ", node_id);
      return Error::RaftError();
    }
  } while (!done);

  return std::nullopt;
}
} // namespace yuzhi::raft_engine::net
