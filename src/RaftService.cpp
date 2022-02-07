/*
 * @Author: Leo
 * @Date: 2022-02-03 16:06:57
 * @LastEditTime: 2022-02-06 13:13:38
 * @LastEditors: Leo
 * @Description:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/src/RaftService.cpp
 */

#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpServer.h>
#include <raft_engine/net/RaftService.hpp>
#include <raft_engine/net/interface_raft.h>
#include <utility/tpl/tpl.h>

using namespace yuzhi::raft_engine::net;
using namespace muduo::net;
using namespace muduo;

using namespace std;

static RaftService *self = nullptr;

static int __connect_if_needed(peer_connection_t *conn) {
  if (CONNECTED != conn->connection_status) {
    if (DISCONNECTED == conn->connection_status)
      self->__connect_to_peer(conn);
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

static void __peer_msg_send(peer_connection_t *conn, tpl_node *tn, Buffer *buf,
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

void RaftService::__delete_connection(peer_connection_t *conn) {

  peer_connection_t *prev = NULL;
  if (this->conns == conn)
    this->conns = conn->next;
  else if (this->conns != conn) {
    for (prev = this->conns; prev->next != conn; prev = prev->next)
      ;
    prev->next = conn->next;
  } else
    assert(0);

  if (conn->node)
    raft_node_set_udata(conn->node, NULL);

  // TODO: make sure all resources are freed
  free(conn);
}

peer_connection_t *
RaftService::__find_connection(const muduo::net::InetAddress &addr) {
  peer_connection_t *conn;
  for (conn = conns; conn && conn->addr == addr; conn = conn->next)
    ;
  return conn;
}

static int __send_handshake_response(int node_id, peer_connection_t *conn,
                                     handshake_state_e success,
                                     raft_node_t *leader, int16_t http_port) {
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

int RaftService::__deserialize_and_handle_msg(void *img, size_t sz,
                                              void *data) {
  peer_connection_t *conn = (peer_connection_t *)data;
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
    e = raft_recv_appendentries((raft_server_t *)raft, conn->node, &conn->ae.ae,
                                &msg.aer);

    /* send response */
    __peer_msg_send(conn, tpl_map("S(I$(IIII))", &msg), bufs, buf);
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

    raft_node_t *leader = raft_get_current_leader_node((raft_server_t *)raft);

    /* Is this peer in our configuration already? */
    raft_node_t *node = raft_get_node((raft_server_t *)raft, m.hs.node_id);
    if (node) {
      raft_node_set_udata(node, conn);
      conn->node = node;
    }

    if (!leader) {
      return __send_handshake_response(node_id, conn, HANDSHAKE_FAILURE, NULL,
                                       http_port);
    } else if (raft_node_get_id(leader) != node_id) {
      return __send_handshake_response(node_id, conn, HANDSHAKE_FAILURE, leader,
                                       http_port);
    } else if (node) {
      return __send_handshake_response(node_id, conn, HANDSHAKE_SUCCESS, NULL,
                                       http_port);
    } else {
      int e = __append_cfg_change(this, RAFT_LOGTYPE_ADD_NONVOTING_NODE,
                                  conn->addr.toIp().data(), m.hs.raft_port,
                                  m.hs.http_port, m.hs.node_id);
      if (0 != e)
        return __send_handshake_response(node_id, conn, HANDSHAKE_FAILURE, NULL,
                                         http_port);
      return __send_handshake_response(node_id, conn, HANDSHAKE_SUCCESS, NULL,
                                       http_port);
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
        conn->node = raft_get_node((raft_server_t *)raft, m.hsr.node_id);
    }
    break;
  case MSG_LEAVE: {
    if (!conn->node) {
      printf("ERROR: no node\n");
      return 0;
    }
    int e = __append_cfg_change(this, RAFT_LOGTYPE_REMOVE_NODE,
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
    e = raft_recv_requestvote((raft_server_t *)raft, conn->node, &m.rv,
                              &msg.rvr);
    string fmt = "S(I$(II))";
    __peer_msg_send(conn, tpl_map(fmt.data(), &msg), bufs, buf);
  } break;
  case MSG_REQUESTVOTE_RESPONSE:
    e = raft_recv_requestvote_response((raft_server_t *)raft, conn->node,
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
      e = raft_recv_appendentries((raft_server_t *)raft, conn->node, &m.ae,
                                  &msg.aer);

      string fmt = "S(I$(IIII))";
      __peer_msg_send(conn, tpl_map(fmt.data(), &msg), bufs, buf);
    }
    break;
  case MSG_APPENDENTRIES_RESPONSE:
    e = raft_recv_appendentries_response((raft_server_t *)raft, conn->node,
                                         &m.aer);
    // uv_cond_signal(&sv->appendentries_received);
    break;
  default:
    printf("unknown msg\n");
    exit(0);
  }
  return 0;
}

int RaftService::__append_cfg_change(RaftService *sv,
                                     raft_logtype_e change_type, char *host,
                                     int raft_port, int http_port,
                                     int node_id) {
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
  if (int e = raft_recv_entry((raft_server_t *)raft, &entry, &r); 0 != e)
    return -1;
  return 0;
}

/** Raft callback for applying an entry to the finite state machine */
// TODO: import this from raft.h
static int __raft_applylog(raft_server_t *raft, void *udata, raft_entry_t *ety,
                           raft_index_t entry_idx) {
  return -1;
}

/** Raft callback for saving voted_for field to disk.
 * This only returns when change has been made to disk. */
static int __raft_persist_vote(raft_server_t *raft, void *udata,
                               const int voted_for) {
  return -1;
}

/** Raft callback for saving term field to disk.
 * This only returns when change has been made to disk. */
static int __raft_persist_term(raft_server_t *raft, void *udata,
                               raft_term_t current_term, raft_node_id_t vote) {
  return -1;
}

/** Raft callback for appending an item to the log */
// TODO: import this from raft.c
static int __raft_logentry_offer(raft_server_t *raft, void *udata,
                                 raft_entry_t *ety, raft_index_t ety_idx) {
  return -1;
}

/** Raft callback for deleting the most recent entry from the log.
 * This happens when an invalid leader finds a valid leader and has to delete
 * superseded log entries. */
int __raft_logentry_poll(raft_server_t *raft, void *udata, raft_entry_t *entry,
                         raft_index_t ety_idx) {
  return 0;
}

/** Raft callback for deleting the most recent entry from the log.
 * This happens when an invalid leader finds a valid leader and has to delete
 * superseded log entries. */
int __raft_logentry_pop(raft_server_t *raft, void *udata, raft_entry_t *entry,
                        raft_index_t ety_idx) {
  return -1;
}

/** Non-voting node now has enough logs to be able to vote.
 * Append a finalization cfg log entry. */
int __raft_node_has_sufficient_logs(raft_server_t *raft, void *user_data,
                                    raft_node_t *node) {
  peer_connection_t *conn = (peer_connection_t *)raft_node_get_udata(node);
  self->__append_cfg_change(self, RAFT_LOGTYPE_ADD_NODE,
                            conn->addr.toIp().data(), conn->raft_port,
                            conn->http_port, raft_node_get_id(conn->node));
  return -1;
}

/** Raft callback for displaying debugging information */
static void __raft_log(raft_server_t *raft, raft_node_t *node, void *udata,
                       const char *buf) {
  spdlog::debug("raft: {}", buf);
}

peer_connection_t *RaftService::__new_connection() {
  peer_connection_t *conn =
      (peer_connection_t *)calloc(1, sizeof(peer_connection_t));
  conn->next = conns;
  conns = conn;
  return conn;
}

void RaftService::__on_connection_accepted_by_peer(
    peer_connection_t *data, const TcpConnectionPtr &conn) {
  spdlog::info("RaftService::__on_connection_accepted_by_peer");

  if (conn->connected()) {
    data->peer = conn;
    data->addr = conn->peerAddress();
    data->connection_status = CONNECTED;
    // __send_handshake
    conn->setContext(conn);
  }
}

void RaftService::__connect_to_peer(peer_connection_t *conn) {
  assert(self);

  cls_.emplace_back(
      std::make_unique<TcpClient>(&loop_, conn->addr, "RaftClient"));
  auto &cl = *cls_.back();
  conn->connection_status = CONNECTING;
  cl.setConnectionCallback(std::bind(
      &RaftService::__on_connection_accepted_by_peer, this, conn, _1));
  cl.connect();
}

static void __connection_set_peer(peer_connection_t *conn, char *host,
                                  int port) {
  conn->raft_port = port;
  spdlog::info("Connecting to {}:{}", host, conn->raft_port);
  muduo::net::InetAddress addr(host, port);
  conn->addr = addr;
}

void RaftService::__connect_to_peer_at_host(peer_connection_t *conn, char *host,
                                            int port) {
  __connection_set_peer(conn, host, port);
  __connect_to_peer(conn);
}

static int __raft_send_appendentries(raft_server_t *raft, void *user_data,
                              raft_node_t *node, msg_appendentries_t *m) {
  Buffer bufs[3];
  auto conn = (peer_connection_t *)raft_node_get_udata(node);

  if (int e = __connect_if_needed(conn); - 1 == e)
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

int __raft_send_requestvote(raft_server_t *raft, void *user_data,
                            raft_node_t *node, msg_requestvote_t *m) {
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
    .send_requestvote = __raft_send_requestvote,
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

RaftService::RaftService() {

  assert(nullptr == self);
  self = this;

  auto &config = Config::Instance();
  auto startable = config.get<bool>(this, "start");
  auto joinable = config.get<bool>(this, "join");
  auto host = config.get<std::string>(this, "host");
  auto raft_port = config.get<int>(this, "raft_port");
  http_port = config.get<int>(this, "http_port");

  raft = raft_new();

  if (startable || joinable) {
    node_id = config.get<int>(this, "node_id");
  } else {
    spdlog::error("Please start or join a cluster.");
    abort();
  }

  raft_set_callbacks((raft_server_t *)raft, &raft_funcs, this);
  raft_add_node((raft_server_t *)raft, NULL, node_id, 1);

  if (startable || joinable) {

    uint16_t port = static_cast<uint16_t>(raft_port);
    InetAddress serverAddr(port);

    server_ = std::move(std::make_unique<muduo::net::TcpServer>(
        &loop_, serverAddr, "RaftServer"));
    server_->setConnectionCallback(
        bind(&RaftService::onConnection, this, std::placeholders::_1));

    if (startable) {
      raft_become_leader((raft_server_t *)raft);
      /* We store membership configuration inside the Raft log.
       * This configuration change is going to be the initial membership
       * configuration (ie. original node) inside the Raft log. The
       * first configuration is for a cluster of 1 node. */
      __append_cfg_change(this, RAFT_LOGTYPE_ADD_NODE, host.data(), (raft_port),
                          (http_port), node_id);
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

  spdlog::info("{} -> {} is {}", conn->localAddress().toIpPort(),
               conn->peerAddress().toIpPort(),
               conn->connected() ? "UP" : "DOWN");

  if (conn->connected()) {
    spdlog::info("Connection established");
    peer_connection_t *conn1 = __new_connection();
    conn1->connection_status = CONNECTED;
    conn1->node = NULL;
    conn1->peer = conn;
    conn1->addr = conn->peerAddress();

    conn->setContext(conn1);
  }
}
