/*
 * @Author: Leo
 * @Date: 2022-02-04 17:46:31
 * @LastEditTime: 2022-08-18 01:12:30
 * @LastEditors: Leo
 */
#pragma once

#ifndef __RAFT_ENGINE_NET_INTERFACE_RAFT_H__
#define __RAFT_ENGINE_NET_INTERFACE_RAFT_H__

extern "C"
{
#include <raft.h>
}
#include <array>
#include <optional>
#include <string.h>
#include <utility/tpl.h>
#include <uv.h>
constexpr auto RAFT_BUFLEN = 512;
constexpr auto IP_STR_LEN = strlen("111.111.111.111");

struct address_t
{
  uint16_t port;
  char host[IP_STR_LEN];
};

using entry_cfg_change_t = struct
{
  uint16_t raft_port;
  uint16_t http_port;
  uint16_t node_id;
  char host[IP_STR_LEN];
};

using peer_message_type_ = enum {
  /** Handshake is a special non-raft message type
   * We send a handshake so that we can identify ourselves to our peers */
  MSG_HANDSHAKE,
  /** Successful responses mean we can start the Raft periodic callback */
  MSG_HANDSHAKE_RESPONSE,
  /** Tell leader we want to leave the cluster */
  /* When instance is ctrl-c'd we have to gracefuly disconnect */
  MSG_LEAVE,
  /* Receiving a leave response means we can shutdown */
  MSG_LEAVE_RESPONSE,
  MSG_REQUESTVOTE,
  MSG_REQUESTVOTE_RESPONSE,
  MSG_APPENDENTRIES,
  MSG_APPENDENTRIES_RESPONSE,
};

enum class conn_status_e
{
  DISCONNECTED,
  CONNECTING,
  CONNECTED,
};

enum class handshake_state_e
{
  HANDSHAKE_FAILURE,
  HANDSHAKE_SUCCESS,
};

using msg_handshake_t = struct
{
  int raft_port;
  int http_port;
  int node_id;
};

using msg_handshake_response_t = struct
{
  handshake_state_e success;
  int leader_port;
  int http_port;
  /* my Raft node ID.
   * Sometimes we don't know who we did the handshake with */
  int node_id;
  char leader_host[IP_STR_LEN];
};

struct msg_t
{
  int type;
  union {
    msg_handshake_t hs;
    msg_handshake_response_t hsr;
    msg_requestvote_t rv;
    msg_requestvote_response_t rvr;
    msg_appendentries_t ae;
    msg_appendentries_response_t aer;
  }; // __internal_msg_t_union_e;

  // #define hs __internal_msg_t_union_e.hs
  // #define hsr __internal_msg_t_union_e.hsr
  // #define rv __internal_msg_t_union_e.rv
  // #define rvr __internal_msg_t_union_e.rvr
  // #define ae __internal_msg_t_union_e.ae
  // #define aer __internal_msg_t_union_e.aer

  int padding[100];
}; // size 456

struct peer_connection_t
{

  /* peer's address */
  struct sockaddr_in addr;

  int http_port;
  int raft_port;

  /* gather TPL message */
  tpl_gather_t *gt;

  /* tell if we need to connect or not */
  conn_status_e connection_status;

  /* peer's raft node_idx */
  raft_node_t *node;

  /* number of entries currently expected.
   * this counts down as we consume entries */
  int n_expected_entries;

  /* remember most recent append entries msg, we refer to this msg when we
   * finish reading the log entries.
   * used in tandem with n_expected_entries */
  msg_t ae;

  uv_stream_t *stream;

  uv_loop_t *loop;

  void *udata;

  struct peer_connection_t *next;
};

enum class RaftRole
{
  UNKNOW,
  LEADER,
  SLAVE,
};

enum class IdleState
{
  RESTORE,
  JOIN,
  START,
};

struct RaftOptions
{
  IdleState idle_state;

  /* flags */
  int debug;

  /* options */
  char *host;
  int id;
  int raft_port;
  int http_port;

  std::optional<address_t> cluster_address;
};

struct RaftServerInterface
{
  /* Link list of peer connections */
  peer_connection_t *conns = nullptr;
  uv_mutex_t raft_lock;
  uv_cond_t appendentries_received;
  int node_id;

  raft_server_t *raft = nullptr;
  uv_loop_t peer_loop;
  uv_timer_t periodic_req;
  uv_tcp_t peer_listen;

  raft_cbs_t raft_callbacks;

  RaftOptions opts;

  void *user_data = nullptr;
};

#endif // __RAFT_ENGINE_NET_INTERFACE_RAFT_H__