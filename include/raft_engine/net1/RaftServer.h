/*
 * @Author: Leo
 * @Date: 2022-08-06 17:21:29
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-07 03:35:02
 */
#pragma once

#ifndef YUZHI_RAFT_ENGINE_RAFT_SERVER_H_
#define YUZHI_RAFT_ENGINE_RAFT_SERVER_H_

extern "C"
{
#include <raft.h>
#include <stdlib.h> // for random
}
#include "uv_helpers.h"
#include <raft_engine/net1/interface_raft.h>
#include <spdlog/spdlog.h>
#include <stdlib.h>
#include <utility/Snow.h>
#include <uv.h>
#include <uv/errno.h>

namespace yuzhi::raft_engine
{

typedef struct
{
  /* commands */
  int drop;
  int join;
  int leave;
  int start;

  /* flags */
  int daemonize;
  int debug;
  int help;
  int version;

  /* options */
  char *db_size;
  char *host;
  char *http_port;
  char *id;
  char *path;
  char *pid_file;
  char *raft_port;

  /* arguments */
  char *PEER;

} options_t;

class RaftServer;

thread_local RaftServer *sv;
options_t opts;

class RaftServer
{
private:
  /* data */
public:
  RaftServer(/* args */);
  ~RaftServer();

private:
  static void __periodic(uv_timer_t *handle)
  {
    auto raftServer = reinterpret_cast<RaftServer *>(handle->data);
    if (raft_periodic(raftServer->raft, PERIOD_MSEC) == -1)
    {
      // SPDLOG_ERROR("Failed to connect to raft server: %d", (int)raftServer->raft);
    }
  }
  constexpr static auto PERIOD_MSEC = 1000;

  /** Serialize a peer message using TPL
   * @param[out] bufs libuv buffer to insert serialized message into
   * @param[out] buf Buffer to write serialized message into */
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
    int e = uv_try_write(s, buf, 1);
    if (e < 0)
      uv_fatal(e);
  }

  static void __send_handshake(peer_connection_t *conn)
  {
    uv_buf_t bufs[1];
    char buf[RAFT_BUFLEN];
    msg_t msg = {};
    msg.type = MSG_HANDSHAKE;
    msg.hs.raft_port = atoi(opts.raft_port);
    msg.hs.http_port = atoi(opts.http_port);
    msg.hs.node_id = sv->node_id;
    __peer_msg_send(conn->stream, tpl_map("S(I$(IIII))", &msg), &bufs[0], buf);
  }

  /** Our connection attempt to raft peer has succeeded */
  static void __on_connection_accepted_by_peer(uv_connect_t *req, const int status)
  {
    peer_connection_t *conn = reinterpret_cast<peer_connection_t *>(req->data);
    int e;

    switch (status)
    {
    case 0:
      break;
    case -ECONNREFUSED:
      return;
    default:
      uv_fatal(status);
    }

    __send_handshake(conn);

    int nlen = sizeof(conn->addr);
    e = uv_tcp_getpeername((uv_tcp_t *)req->handle, (struct sockaddr *)&conn->addr, &nlen);
    if (0 != e)
      uv_fatal(e);

    /* start reading from peer */
    conn->connection_status = CONNECTED;
    e = uv_read_start(conn->stream, __peer_alloc_cb, __peer_read_cb);
    if (0 != e)
      uv_fatal(e);
  }

  /** Read raft traffic using binary protocol */
  static void __peer_read_cb(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf)
  {
    peer_connection_t *conn = reinterpret_cast<peer_connection_t *>(tcp->data);

    if (nread < 0)
      switch (nread)
      {
      // case UV__ECONNRESET: // windows
      case UV__EOF:
        conn->connection_status = DISCONNECTED;
        return;
      default:
        uv_fatal(nread);
      }

    if (0 <= nread)
    {
      assert(conn);
      uv_mutex_lock(&sv->raft_lock);
      tpl_gather(TPL_GATHER_MEM, buf->base, nread, &conn->gt, __deserialize_and_handle_msg, conn);
      uv_mutex_unlock(&sv->raft_lock);
    }
  }

  static int __send_handshake_response(peer_connection_t *conn, handshake_state_e success, raft_node_t *leader)
  {
    uv_buf_t bufs[1];
    char buf[RAFT_BUFLEN];

    msg_t msg = {};
    msg.type = MSG_HANDSHAKE_RESPONSE;
    msg.hsr.success = success;
    msg.hsr.leader_port = 0;
    msg.hsr.node_id = sv->node_id;

    /* allow the peer to redirect to the leader */
    if (leader)
    {
      peer_connection_t *leader_conn = reinterpret_cast<peer_connection_t *>(raft_node_get_udata(leader));
      if (leader_conn)
      {
        msg.hsr.leader_port = leader_conn->raft_port;
        snprintf(msg.hsr.leader_host, IP_STR_LEN, "%s", inet_ntoa(leader_conn->addr.sin_addr));
      }
    }

    msg.hsr.http_port = atoi(opts.http_port);

    __peer_msg_send(conn->stream, tpl_map("S(I$(IIIIs))", &msg), bufs, buf);

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

  static peer_connection_t *__find_connection(RaftServer *sv, const char *host, int raft_port)
  {
    peer_connection_t *conn;
    for (conn = sv->conns; conn && (0 != strcmp(host, inet_ntoa(conn->addr.sin_addr)) || conn->raft_port != raft_port);
         conn = conn->next)
      ;
    return conn;
  }

  static void __drop_db(RaftServer *sv) {}
  static int __append_cfg_change(RaftServer *sv, raft_logtype_e change_type, char *host, int raft_port, int http_port,
                                 int node_id)
  {
    entry_cfg_change_t *change = (entry_cfg_change_t *)calloc(1, sizeof(*change));
    change->raft_port = raft_port;
    change->http_port = http_port;
    change->node_id = node_id;
    strcpy(change->host, host);
    change->host[IP_STR_LEN - 1] = 0;

    msg_entry_t entry;
    entry.id = rand();
    entry.data.buf = (void *)change;
    entry.data.len = sizeof(*change);
    entry.type = change_type;
    msg_entry_response_t r;
    int e = raft_recv_entry(sv->raft, &entry, &r);
    if (0 != e)
      return -1;
    return 0;
  }

  static peer_connection_t *__new_connection(RaftServer *sv)
  {
    peer_connection_t *conn = (peer_connection_t *)calloc(1, sizeof(peer_connection_t));
    conn->loop = &sv->peer_loop;
    conn->next = sv->conns;
    sv->conns = conn;
    return conn;
  }

  static void __delete_connection(RaftServer *sv, peer_connection_t *conn)
  {
    peer_connection_t *prev = NULL;
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
      raft_node_set_udata(conn->node, NULL);

    // TODO: make sure all resources are freed
    free(conn);
  }

  static void __connection_set_peer(peer_connection_t *conn, char *host, int port)
  {
    conn->raft_port = port;
    printf("Connecting to %s:%d\n", host, port);
    int e = uv_ip4_addr(host, port, &conn->addr);
    if (0 != e)
      uv_fatal(e);
  }

  static void __connect_to_peer_at_host(peer_connection_t *conn, char *host, int port)
  {
    __connection_set_peer(conn, host, port);
    __connect_to_peer(conn);
  }

  /** Parse raft peer traffic using binary protocol, and respond to message */
  static int __deserialize_and_handle_msg(void *img, size_t sz, void *data)
  {
    peer_connection_t *conn = reinterpret_cast<peer_connection_t *>(data);
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
      peer_connection_t *nconn = __find_connection(sv, inet_ntoa(conn->addr.sin_addr), m.hs.raft_port);
      if (nconn && conn != nconn)
        __delete_connection(sv, nconn);

      conn->connection_status = CONNECTED;
      conn->http_port = m.hs.http_port;
      conn->raft_port = m.hs.raft_port;

      raft_node_t *leader = raft_get_current_leader_node(sv->raft);

      /* Is this peer in our configuration already? */
      raft_node_t *node = raft_get_node(sv->raft, m.hs.node_id);
      if (node)
      {
        raft_node_set_udata(node, conn);
        conn->node = node;
      }

      if (!leader)
      {
        return __send_handshake_response(conn, HANDSHAKE_FAILURE, NULL);
      }
      else if (raft_node_get_id(leader) != sv->node_id)
      {
        return __send_handshake_response(conn, HANDSHAKE_FAILURE, leader);
      }
      else if (node)
      {
        return __send_handshake_response(conn, HANDSHAKE_SUCCESS, NULL);
      }
      else
      {
        int e = __append_cfg_change(sv, RAFT_LOGTYPE_ADD_NONVOTING_NODE, inet_ntoa(conn->addr.sin_addr), m.hs.raft_port,
                                    m.hs.http_port, m.hs.node_id);
        if (0 != e)
          return __send_handshake_response(conn, HANDSHAKE_FAILURE, NULL);
        return __send_handshake_response(conn, HANDSHAKE_SUCCESS, NULL);
      }
    }
    break;
    case MSG_HANDSHAKE_RESPONSE:
      if (0 == m.hsr.success)
      {
        conn->http_port = m.hsr.http_port;

        /* We're being redirected to the leader */
        if (m.hsr.leader_port)
        {
          peer_connection_t *nconn = __find_connection(sv, m.hsr.leader_host, m.hsr.leader_port);
          if (!nconn)
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
        printf("ERROR: no node\n");
        return 0;
      }
      int e = __append_cfg_change(sv, RAFT_LOGTYPE_REMOVE_NODE, inet_ntoa(conn->addr.sin_addr), conn->raft_port,
                                  conn->http_port, raft_node_get_id(conn->node));
      if (0 != e)
        printf("ERROR: Leave request failed\n");
    }
    break;
    case MSG_LEAVE_RESPONSE:
      __drop_db(sv);
      printf("Shutdown complete. Quitting...\n");
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

  static void __peer_alloc_cb(uv_handle_t *handle, size_t size, uv_buf_t *buf)
  {
    buf->len = size;
    buf->base = (char *)malloc(size);
  }

  /** Connect to raft peer */
  static void __connect_to_peer(peer_connection_t *conn)
  {
    int e;

    uv_tcp_t *tcp = reinterpret_cast<uv_tcp_t *>(calloc(1, sizeof(uv_tcp_t)));
    tcp->data = conn;
    e = uv_tcp_init(conn->loop, tcp);
    if (0 != e)
      uv_fatal(e);

    conn->stream = (uv_stream_t *)tcp;
    conn->connection_status = CONNECTING;

    uv_connect_t *c = reinterpret_cast<uv_connect_t *>(calloc(1, sizeof(uv_connect_t)));
    c->data = conn;

    e = uv_tcp_connect(c, (uv_tcp_t *)conn->stream, (struct sockaddr *)&conn->addr, __on_connection_accepted_by_peer);
    if (0 != e)
      uv_fatal(e);
  }

  /** Initiate connection if we are disconnected */
  static int __connect_if_needed(peer_connection_t *conn)
  {
    if (CONNECTED != conn->connection_status)
    {
      if (DISCONNECTED == conn->connection_status)
        __connect_to_peer(conn);
      return -1;
    }
    return 0;
  }

  static int __raft_send_appendentries(raft_server_t *raft, void *user_data, raft_node_t *node, msg_appendentries_t *m)
  {
    uv_buf_t bufs[3];
    peer_connection_t *conn = reinterpret_cast<peer_connection_t *>(raft_node_get_udata(node));

    int e = __connect_if_needed(conn);
    if (-1 == e)
      return 0;

    char buf[RAFT_BUFLEN], *ptr = buf;
    msg_t msg = {};
    msg.type = MSG_APPENDENTRIES;
    msg.ae.term = m->term;
    msg.ae.prev_log_idx = m->prev_log_idx;
    msg.ae.prev_log_term = m->prev_log_term;
    msg.ae.leader_commit = m->leader_commit;
    msg.ae.n_entries = m->n_entries;
    ptr += __peer_msg_serialize(tpl_map("S(I$(IIIII))", &msg), bufs, ptr);

    /* appendentries with payload */
    if (0 < m->n_entries)
    {
      tpl_bin tb;
      tb.sz = m->entries[0].data.len;
      tb.addr = m->entries[0].data.buf;

      /* list of entries */
      tpl_node *tn = tpl_map("IIIB", &m->entries[0].id, &m->entries[0].term, &m->entries[0].type, &tb);
      size_t sz;
      tpl_pack(tn, 0);
      tpl_dump(tn, TPL_GETSIZE, &sz);
      e = tpl_dump(tn, TPL_MEM | TPL_PREALLOCD, ptr, RAFT_BUFLEN);
      assert(0 == e);
      bufs[1].len = sz;
      bufs[1].base = ptr;
      e = uv_try_write(conn->stream, bufs, 2);
      if (e < 0)
        uv_fatal(e);

      tpl_free(tn);
    }
    else
    {
      /* keep alive appendentries only */
      e = uv_try_write(conn->stream, bufs, 1);
      if (e < 0)
        uv_fatal(e);
    }

    return 0;
  }

  static int __send_requestvote(raft_server_t *raft, void *udata, raft_node_t *node, msg_requestvote_t *m)
  {
    peer_connection_t *conn = reinterpret_cast<peer_connection_t *>(raft_node_get_udata(node));

    uv_buf_t bufs[1];
    char buf[RAFT_BUFLEN];
    msg_t msg = {.type = MSG_REQUESTVOTE, .rv = *m};
    __peer_msg_serialize(tpl_map("S(I$(IIII))", &msg), bufs, buf);
    int e = uv_try_write(conn->stream, bufs, 1);
    if (e < 0)
    {
      uv_fatal(e);
    }
    return 0;
  }

private:
  void timer_init()
  {
    periodic_req.data = this;
    uv_timer_init(&peer_loop, &periodic_req);
    uv_timer_start(&periodic_req, __periodic, 0, 1000);
  }

public:
  /* Link list of peer connections */
  peer_connection_t *conns;
  uv_mutex_t raft_lock;
  uv_cond_t appendentries_received;
  int node_id;

private:
  raft_server_t *raft;
  uv_loop_t peer_loop;
  uv_timer_t periodic_req;
};

RaftServer::RaftServer(/* args */) : raft{raft_new()}
{
  timer_init();
  auto connection_user_data = this;
  auto node_id = random() % (1 << utility::workerIdBits);
  raft_add_node(raft, connection_user_data, node_id, true);
}

RaftServer::~RaftServer() {}

} // namespace yuzhi::raft_engine

#endif