/*
 * @Author: Leo
 * @Date: 2022-08-06 17:21:29
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-09 12:13:16
 */
#pragma once

#ifndef YUZHI_RAFT_ENGINE_RAFT_SERVER_H_
#define YUZHI_RAFT_ENGINE_RAFT_SERVER_H_

extern "C"
{
#include <raft.h>
}
#include "uv_helpers.h"
#include <any>
#include <raft_engine/net1/interface_raft.h>
#include <spdlog/spdlog.h>
#include <stdlib.h> // for random
#include <stdlib.h>
#include <utility/Snow.h>
#include <uv.h>
#include <uv/errno.h>

namespace yuzhi::raft_engine
{
using options_t = struct
{
  int join;
  int leave;
  int start;

  /* flags */
  int debug;

  /* options */
  char *host;
  char *http_port;
  char *id;
  char *raft_port;

  /* arguments */
  char *PEER;
};

static options_t opts;

class RaftServer
{
private:
  /* data */
public:
  RaftServer(int raft_port, int engine_port, bool join = false);
  ~RaftServer();

public:
  void leave();
  void sync(std::any y);

private:
#pragma region // intrnal functions
  static int __connect_if_needed(peer_connection_t *conn);
  static void __peer_read_cb(uv_stream_t *tcp, ssize_t nread, const uv_buf_t *buf);
  static void __on_connection_accepted_by_peer(uv_connect_t *req, const int status);
  static int __offer_cfg_change(raft_server_t *raft, void *data, raft_logtype_e change_type);
  static int __raft_node_has_sufficient_logs(raft_server_t *raft, void *user_data, raft_node_t *node);
  static peer_connection_t *__new_connection(RaftServer *sv);
  static void __delete_connection(RaftServer *sv, peer_connection_t *conn);

  static void __connection_set_peer(peer_connection_t *conn, char *host, int port);

  static void __connect_to_peer(peer_connection_t *conn);
  static void __connect_to_peer_at_host(peer_connection_t *conn, char *host, int port);

  /** Parse raft peer traffic using binary protocol, and respond to message */
  static int __deserialize_and_handle_msg(void *img, size_t sz, void *data);

  static int __raft_send_appendentries(raft_server_t *raft, void *user_data, raft_node_t *node, msg_appendentries_t *m);

  static int __append_cfg_change(RaftServer *sv, raft_logtype_e change_type, const char *host, uint16_t raft_port,
                                 uint16_t http_port, uint16_t id);
#pragma endregion
private:
  constexpr static auto PERIOD_MSEC = 1000;
  static void __periodic(uv_timer_t *handle);

  static int __send_requestvote(raft_server_t *raft, void *udata, raft_node_t *node, msg_requestvote_t *m);

private:
  void timer_init();
  void callback_init();
  void __start_peer_socket(const char *host, int port, uv_tcp_t *listen);
  static void __on_peer_connection(uv_stream_t *listener, const int status);

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
} // namespace yuzhi::raft_engine

#endif