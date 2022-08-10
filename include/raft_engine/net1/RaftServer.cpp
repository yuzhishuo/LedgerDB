/*
 * @Author: Leo
 * @Date: 2022-08-07 16:19:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-09 12:19:20
 */
#include "RaftServer.h"
#include <array>
#include <string>

namespace yuzhi::raft_engine
{

constexpr auto MAX_PEER_CONNECTIONS = 128;
constexpr auto MAX_HTTP_CONNECTIONS = 128;

static thread_local RaftServer *sv = nullptr;

static void __peer_alloc_cb([[maybe_unused]] uv_handle_t *handle, size_t size, uv_buf_t *buf)
{
  buf->len = size;
  buf->base = (char *)malloc(size);
}

static size_t __peer_msg_serialize(tpl_node *tn, uv_buf_t *buf, char *data)
{
  size_t sz;
  tpl_pack(tn, 0);
  tpl_dump(tn, TPL_GETSIZE, &sz);
  tpl_dump(tn, TPL_MEM | TPL_PREALLOCD, data, RAFT_BUFLEN);
  tpl_free(tn);
  buf->len = sz;
  buf->base = data;
  return sz;
}

static void __peer_msg_send(uv_stream_t *s, tpl_node *tn, uv_buf_t *buf, char *data)
{
  __peer_msg_serialize(tn, buf, data);
  if (int e = uv_try_write(s, buf, 1); e < 0)
  {
    uv_fatal(e)
  }
}

peer_connection_t *RaftServer::__new_connection(RaftServer *sv)
{
  auto conn = static_cast<peer_connection_t *>(malloc(sizeof(peer_connection_t)));
  memset(conn, 0, sizeof(peer_connection_t));
  conn->loop = &sv->peer_loop;
  conn->next = sv->conns;
  sv->conns = conn;
  return conn;
}

void RaftServer::__delete_connection(RaftServer *sv, peer_connection_t *conn)
{
  peer_connection_t *prev = nullptr;
  if (sv->conns == conn)
    sv->conns = conn->next;
  else if (sv->conns != conn)
  {
    for (prev = sv->conns; prev->next != conn; prev = prev->next)
      ;
    prev->next = conn->next;
  }
  else
    assert(0);

  if (conn->node)
    raft_node_set_udata(conn->node, nullptr);

  // TODO: make sure all resources are freed
  free(conn);
}

static int __send_handshake_response(peer_connection_t *conn, handshake_state_e success, raft_node_t *leader)
{
  uv_buf_t bufs;
  std::array<char, RAFT_BUFLEN> buf;

  msg_t msg = {};
  msg.type = MSG_HANDSHAKE_RESPONSE;
  msg.hsr.success = success;
  msg.hsr.leader_port = 0;
  msg.hsr.node_id = sv->node_id;

  /* allow the peer to redirect to the leader */
  if (leader)
  {
    if (auto leader_conn = static_cast<peer_connection_t *>(raft_node_get_udata(leader)); leader_conn)
    {
      msg.hsr.leader_port = leader_conn->raft_port;
      snprintf(msg.hsr.leader_host, IP_STR_LEN, "%s", inet_ntoa(leader_conn->addr.sin_addr));
    }
  }

  msg.hsr.http_port = atoi(opts.http_port);

  __peer_msg_send(conn->stream, tpl_map("S(I$(IIIIs))", &msg), &bufs, buf.data());

  return 0;
}

static void __send_handshake(peer_connection_t *conn)
{
  uv_buf_t bufs;
  std::array<char, RAFT_BUFLEN> buf;
  msg_t msg = {};
  msg.type = MSG_HANDSHAKE;
  msg.hs.raft_port = atoi(opts.raft_port);
  msg.hs.http_port = atoi(opts.http_port);
  msg.hs.node_id = sv->node_id;
  __peer_msg_send(conn->stream, tpl_map("S(I$(IIII))", &msg), &bufs, buf.data());
}

static void __connection_set_peer(peer_connection_t *conn, char *host, int port)
{
  conn->raft_port = port;
  printf("Connecting to %s:%d\n", host, port);

  if (auto e = uv_ip4_addr(host, port, &conn->addr); 0 != e)
  {
    SPDLOG_ERROR("Failed to resolve address: {}", uv_strerror(e));
    uv_fatal(e)
  }
}

/** Our connection attempt to raft peer has succeeded */
void RaftServer::__on_connection_accepted_by_peer(uv_connect_t *req, const int status)
{
  auto conn = static_cast<peer_connection_t *>(req->data);

  switch (status)
  {
  case 0:
    break;
  case -ECONNREFUSED:
    return;
  default:
    uv_fatal(status)
  }

  __send_handshake(conn);

  int nlen = sizeof(conn->addr);

  if (auto e = uv_tcp_getpeername((uv_tcp_t *)req->handle, (struct sockaddr *)&conn->addr, &nlen); 0 != e)
  {
    SPDLOG_INFO("Failed to get peer name: {}", uv_strerror(e));
    uv_fatal(e)
  }

  /* start reading from peer */
  conn->connection_status = CONNECTED;

  if (auto e = uv_read_start(conn->stream, __peer_alloc_cb, __peer_read_cb); 0 != e)
  {
    SPDLOG_ERROR("Failed to start read on peer: {}", uv_strerror(e));
    uv_fatal(e)
  }
}

/** Connect to raft peer */
void RaftServer::__connect_to_peer(peer_connection_t *conn)
{

  auto tcp = static_cast<uv_tcp_t *>(calloc(1, sizeof(uv_tcp_t)));
  tcp->data = conn;

  if (auto e = uv_tcp_init(conn->loop, tcp); 0 != e)
  {
    uv_fatal(e)
  }

  conn->stream = (uv_stream_t *)tcp;
  conn->connection_status = CONNECTING;

  auto c = static_cast<uv_connect_t *>(calloc(1, sizeof(uv_connect_t)));
  c->data = conn;

  if (auto e =
          uv_tcp_connect(c, (uv_tcp_t *)conn->stream, (struct sockaddr *)&conn->addr, __on_connection_accepted_by_peer);
      0 != e)
    uv_fatal(e)
}

void RaftServer::__connect_to_peer_at_host(peer_connection_t *conn, char *host, int port)
{
  __connection_set_peer(conn, host, port);
  __connect_to_peer(conn);
}

static peer_connection_t *__find_connection(RaftServer *sv, const char *host, int raft_port)
{
  peer_connection_t *conn;
  for (conn = sv->conns; conn && (0 != strcmp(host, inet_ntoa(conn->addr.sin_addr)) || conn->raft_port != raft_port);
       conn = conn->next)
    ;
  return conn;
}

int RaftServer::__append_cfg_change(RaftServer *sv, raft_logtype_e change_type, const char *host, uint16_t raft_port,
                                    uint16_t http_port, uint16_t id)
{
  auto change = static_cast<entry_cfg_change_t *>(calloc(1, sizeof(entry_cfg_change_t)));
  change->raft_port = raft_port;
  change->http_port = http_port;
  change->node_id = id;
  strcpy(change->host, host);
  change->host[IP_STR_LEN - 1] = 0;

  msg_entry_t entry;
  // TODO: id should be generated by the server
  entry.id = rand();
  entry.data.buf = (void *)change;
  entry.data.len = sizeof(*change);
  entry.type = change_type;
  msg_entry_response_t r;

  if (int e = raft_recv_entry(sv->raft, &entry, &r); 0 != e)
  {
    return -1;
  }

  return 0;
}

int RaftServer::__offer_cfg_change(raft_server_t *raft, void *data, raft_logtype_e change_type)
{
  auto change = static_cast<entry_cfg_change_t *>(data);
  auto conn = __find_connection(sv, change->host, change->raft_port);

  /* Node is being removed */
  if (RAFT_LOGTYPE_REMOVE_NODE == change_type)
  {
    raft_remove_node(raft, raft_get_node(sv->raft, change->node_id));
    if (conn)
      conn->node = NULL;
    /* __delete_connection(sv, conn); */
    return 0;
  }

  /* Node is being added */
  if (!conn)
  {
    conn = __new_connection(sv);
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

int RaftServer::__send_requestvote([[maybe_unused]] raft_server_t *raft, [[maybe_unused]] void *udata,
                                   raft_node_t *node, msg_requestvote_t *m)
{
  SPDLOG_INFO("__send_requestvote");
  auto conn = static_cast<peer_connection_t *>(raft_node_get_udata(node));

  SPDLOG_INFO("__send_requestvote: {}:{}", conn->addr.sin_addr.s_addr, conn->raft_port);

  uv_buf_t bufs;
  std::array<char, RAFT_BUFLEN> buf;
  msg_t msg = {.type = MSG_REQUESTVOTE, .rv = *m};
  __peer_msg_serialize(tpl_map("S(I$(IIII))", &msg), &bufs, buf.data());
  if (auto e = uv_try_write(conn->stream, &bufs, 1); e < 0)
  {
    SPDLOG_ERROR("Failed to send requestvote: {}", uv_strerror(e));
    uv_fatal(e)
  }
  return 0;
}

/** Read raft traffic using binary protocol */
void RaftServer::__peer_read_cb(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf)
{
  auto *conn = static_cast<peer_connection_t *>(tcp->data);

  if (nread < 0)
    switch (nread)
    {
    case UV__EOF:
      conn->connection_status = DISCONNECTED;
      return;
    default:
      uv_fatal(nread)
    }

  if (0 <= nread)
  {
    assert(conn);
    uv_mutex_lock(&sv->raft_lock);
    tpl_gather(TPL_GATHER_MEM, buf->base, nread, &conn->gt, __deserialize_and_handle_msg, conn);
    uv_mutex_unlock(&sv->raft_lock);
  }
}

/** Initiate connection if we are disconnected */
int RaftServer::__connect_if_needed(peer_connection_t *conn)
{
  if (CONNECTED != conn->connection_status)
  {
    if (DISCONNECTED == conn->connection_status)
      __connect_to_peer(conn);
    return -1;
  }
  return 0;
}

int RaftServer::__raft_send_appendentries([[maybe_unused]] raft_server_t *raft, void *user_data, raft_node_t *node,
                                          msg_appendentries_t *m)
{

  SPDLOG_INFO("__raft_send_appendentries");

  auto conn = static_cast<peer_connection_t *>(raft_node_get_udata(node));
  SPDLOG_INFO("Sending appendentries to {}", conn->addr.sin_addr.s_addr);
  if (auto e = __connect_if_needed(conn); - 1 == e)
  {
    SPDLOG_INFO("reconnect {}", conn->addr.sin_addr.s_addr);
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
    auto e = tpl_dump(tn, TPL_MEM | TPL_PREALLOCD, ptr, RAFT_BUFLEN);
    assert(0 == e);
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

static int __send_leave_response(peer_connection_t *conn)
{
  if (!conn)
  {
    SPDLOG_ERROR("no connection??\n");
    return -1;
  }
  if (!conn->stream)
  {
    SPDLOG_ERROR("no stream??\n");
    return -1;
  }

  uv_buf_t bufs;
  std::array<char, RAFT_BUFLEN> buf;
  msg_t msg = {.type = MSG_LEAVE_RESPONSE};
  __peer_msg_send(conn->stream, tpl_map("S(I)", &msg), &bufs, buf.data());
  return 0;
}

/** Deserialize a single log entry from appendentries message */
static void __deserialize_appendentries_payload(msg_entry_t *out, peer_connection_t *conn, void *img, size_t sz)
{
  tpl_bin tb;
  tpl_node *tn = tpl_map(tpl_peek(TPL_MEM, img, sz), &out->id, &out->term, &out->type, &tb);
  tpl_load(tn, TPL_MEM, img, sz);
  tpl_unpack(tn, 0);
  out->data.buf = tb.addr;
  out->data.len = tb.sz;
}

/** Parse raft peer traffic using binary protocol, and respond to message */
int RaftServer::__deserialize_and_handle_msg(void *img, size_t sz, void *data)
{
  peer_connection_t *conn = static_cast<peer_connection_t *>(data);
  msg_t m;
  int e;

  uv_buf_t bufs[1];
  char buf[RAFT_BUFLEN];

  /* special case: handle appendentries payload */
  if (0 < conn->n_expected_entries)
  {
    msg_entry_t entry;

    __deserialize_appendentries_payload(&entry, conn, img, sz);

    conn->ae.ae.entries = &entry;
    msg_t msg = {.type = MSG_APPENDENTRIES_RESPONSE};
    e = raft_recv_appendentries(sv->raft, conn->node, &conn->ae.ae, &msg.aer);

    /* send response */
    uv_buf_t bufs[1];
    char buf[RAFT_BUFLEN];
    __peer_msg_send(conn->stream, tpl_map("S(I$(IIII))", &msg), bufs, buf);

    conn->n_expected_entries = 0;
    return 0;
  }

  /* deserialize message */
  tpl_node *tn = tpl_map(tpl_peek(TPL_MEM, img, sz), &m);
  tpl_load(tn, TPL_MEM, img, sz);
  tpl_unpack(tn, 0);

  switch (m.type)
  {
  case MSG_HANDSHAKE:
  {
    if (auto nconn = __find_connection(sv, inet_ntoa(conn->addr.sin_addr), m.hs.raft_port); nconn && conn != nconn)
      __delete_connection(sv, nconn);

    conn->connection_status = CONNECTED;
    conn->http_port = m.hs.http_port;
    conn->raft_port = m.hs.raft_port;

    auto leader = raft_get_current_leader_node(sv->raft);

    /* Is this peer in our configuration already? */
    raft_node_t *node = raft_get_node(sv->raft, m.hs.node_id);
    if (node)
    {
      raft_node_set_udata(node, conn);
      conn->node = node;
    }

    if (!leader)
    {
      return __send_handshake_response(conn, HANDSHAKE_FAILURE, nullptr);
    }
    else if (raft_node_get_id(leader) != sv->node_id)
    {
      return __send_handshake_response(conn, HANDSHAKE_FAILURE, leader);
    }
    else if (node)
    {
      return __send_handshake_response(conn, HANDSHAKE_SUCCESS, nullptr);
    }
    else
    {
      if (auto e = __append_cfg_change(sv, RAFT_LOGTYPE_ADD_NONVOTING_NODE, inet_ntoa(conn->addr.sin_addr),
                                       m.hs.raft_port, m.hs.http_port, m.hs.node_id);
          0 != e)
        return __send_handshake_response(conn, HANDSHAKE_FAILURE, nullptr);
      return __send_handshake_response(conn, HANDSHAKE_SUCCESS, nullptr);
    }
    break;
  }
  case MSG_HANDSHAKE_RESPONSE:
    if (0 == m.hsr.success)
    {
      conn->http_port = m.hsr.http_port;

      /* We're being redirected to the leader */
      if (m.hsr.leader_port)
      {
        if (auto nconn = __find_connection(sv, m.hsr.leader_host, m.hsr.leader_port); !nconn)
        {
          nconn = __new_connection(sv);
          printf("Redirecting to %s:%d...\n", m.hsr.leader_host, m.hsr.leader_port);
          __connect_to_peer_at_host(nconn, m.hsr.leader_host, m.hsr.leader_port);
        }
      }
    }
    else
    {
      printf("Connected to leader: %s:%d\n", inet_ntoa(conn->addr.sin_addr), conn->raft_port);
      if (!conn->node)
        conn->node = raft_get_node(sv->raft, m.hsr.node_id);
    }
    break;
  case MSG_LEAVE:
  {
    if (!conn->node)
    {
      SPDLOG_ERROR("ERROR: no node\n");
      return 0;
    }
    if (auto e = __append_cfg_change(sv, RAFT_LOGTYPE_REMOVE_NODE, inet_ntoa(conn->addr.sin_addr), conn->raft_port,
                                     conn->http_port, raft_node_get_id(conn->node));
        0 != e)
      SPDLOG_ERROR("ERROR: Leave request failed\n");
  }
  break;
  case MSG_LEAVE_RESPONSE:
    SPDLOG_INFO("Shutdown complete. Quitting...\n");
    exit(0);
    break;
  case MSG_REQUESTVOTE:
  {
    msg_t msg = {.type = MSG_REQUESTVOTE_RESPONSE};
    e = raft_recv_requestvote(sv->raft, conn->node, &m.rv, &msg.rvr);
    __peer_msg_send(conn->stream, tpl_map("S(I$(II))", &msg), bufs, buf);
  }
  break;
  case MSG_REQUESTVOTE_RESPONSE:
    e = raft_recv_requestvote_response(sv->raft, conn->node, &m.rvr);
    break;
  case MSG_APPENDENTRIES:
  {
    /* special case: get ready to handle appendentries payload */
    if (0 < m.ae.n_entries)
    {
      conn->n_expected_entries = m.ae.n_entries;
      memcpy(&conn->ae, &m, sizeof(msg_t));
      return 0;
    }

    /* this is a keep alive message */
    msg_t msg = {.type = MSG_APPENDENTRIES_RESPONSE};
    e = raft_recv_appendentries(sv->raft, conn->node, &m.ae, &msg.aer);
    __peer_msg_send(conn->stream, tpl_map("S(I$(IIII))", &msg), bufs, buf);
    break;
  }
  case MSG_APPENDENTRIES_RESPONSE:
    e = raft_recv_appendentries_response(sv->raft, conn->node, &m.aer);
    uv_cond_signal(&sv->appendentries_received);
    break;
  default:
    printf("unknown msg\n");
    exit(0);
  }
  return 0;
}

static int __raft_persist_vote(raft_server_t *raft, void *udata, raft_node_id_t vote)
{
  SPDLOG_INFO("__raft_persist_vote");
  return 0;
}

static int __raft_persist_term(raft_server_t *raft, void *user_data, raft_term_t term, raft_node_id_t vote)
{
  SPDLOG_INFO("__raft_persist_term");
  return 0;
}

static int __raft_applylog(raft_server_t *raft, [[maybe_unused]] void *user_data, raft_entry_t *entry,
                           raft_index_t entry_idx)
{

  SPDLOG_INFO("Applying log entry {}", entry_idx);
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

static int __raft_logentry_offer(raft_server_t *raft, void *udata, raft_entry_t *ety, raft_index_t entry_idx)
{
  SPDLOG_INFO("__raft_logentry_offer");
  return -1;
}

static int __raft_logentry_poll(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx)
{

  SPDLOG_INFO("__raft_logentry_poll");
  return -1;
}

static int __raft_logentry_pop(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx)
{
  SPDLOG_INFO("__raft_logentry_pop");
  return -1;
}

/** Non-voting node now has enough logs to be able to vote.
 * Append a finalization cfg log entry. */
int RaftServer::__raft_node_has_sufficient_logs(raft_server_t *raft, void *user_data, raft_node_t *node)
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
  SPDLOG_DEBUG("raft: %s\n", buf);
}

/** Raft peer has connected to us.
 * Add them to our list of nodes */
void RaftServer::__on_peer_connection(uv_stream_t *listener, const int status)
{
  int e;

  if (0 != status)
    uv_fatal(status)

        auto tcp = static_cast<uv_tcp_t *>(calloc(1, sizeof(uv_tcp_t)));
  e = uv_tcp_init(listener->loop, tcp);
  if (0 != e)
  {
    uv_fatal(e)
  }
  e = uv_accept(listener, (uv_stream_t *)tcp);
  if (0 != e)
  {
    uv_fatal(e)
  }
  peer_connection_t *conn = __new_connection(sv);
  conn->node = NULL;
  conn->loop = listener->loop;
  conn->stream = (uv_stream_t *)tcp;
  tcp->data = conn;

  int namelen = sizeof(conn->addr);
  e = uv_tcp_getpeername(tcp, (struct sockaddr *)&conn->addr, &namelen);
  if (0 != e)
    uv_fatal(e)

        e = uv_read_start((uv_stream_t *)tcp, __peer_alloc_cb, __peer_read_cb);
  if (0 != e)
    uv_fatal(e)
}

RaftServer::RaftServer(int raft_port, int engine_port, bool join)
{

  raft = raft_new();

  auto connection_user_data = this;
  node_id = random() % (1 << utility::workerIdBits);
  assert(node_id >= 0 and node_id < 32);

  uv_mutex_init(&raft_lock);
  uv_cond_init(&appendentries_received);

  raft_add_node(raft, connection_user_data, node_id, true);

  if (sv != nullptr)
  {
    SPDLOG_ERROR("raft server already initialized");
    exit(1);
  }
  sv = this;

  callback_init();
  timer_init();
  uv_run(&peer_loop, UV_RUN_DEFAULT);
}

RaftServer::~RaftServer() { raft_free(raft); }

void RaftServer::__start_peer_socket(const char *host, int port, uv_tcp_t *listen)
{
  memset(&sv->peer_loop, 0, sizeof(uv_loop_t));
  int e = uv_loop_init(&sv->peer_loop);
  if (0 != e)
  {
    SPDLOG_ERROR("uv_loop_init: {}", uv_strerror(e));
    uv_fatal(e)
  }

  uv_bind_listen_socket(listen, host, port, &sv->peer_loop);
  e = uv_listen((uv_stream_t *)listen, MAX_PEER_CONNECTIONS, __on_peer_connection);
  if (0 != e)
  {
    SPDLOG_ERROR("uv_listen: {}", uv_strerror(e));
    uv_fatal(e)
  }
}

void RaftServer::timer_init()
{
  periodic_req.data = this;
  uv_timer_init(&peer_loop, &periodic_req);
  uv_timer_start(&periodic_req, __periodic, 0, 1000);
  raft_set_election_timeout(raft, 2000);
}

void RaftServer::callback_init()
{
  static raft_cbs_t raft_callbacks = {
      .send_requestvote = __send_requestvote,
      .send_appendentries = __raft_send_appendentries,
      .applylog = __raft_applylog,
      .persist_vote = __raft_persist_vote,
      .persist_term = __raft_persist_term,
      .log_offer = __raft_logentry_offer,
      .log_poll = __raft_logentry_poll,
      .log_pop = __raft_logentry_pop,
      .node_has_sufficient_logs = __raft_node_has_sufficient_logs,
      .log = __raft_log,
  };
  raft_set_callbacks(raft, &raft_callbacks, this);
}

void RaftServer::__periodic(uv_timer_t *handle)
{
  auto raftServer = static_cast<RaftServer *>(handle->data);
  if (raft_periodic(raftServer->raft, PERIOD_MSEC) == -1)
  {
    SPDLOG_ERROR("Failed to connect to raft server: {}", reinterpret_cast<uint64_t>(raftServer->raft));
  }
}

} // namespace yuzhi::raft_engine