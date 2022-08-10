/*
 * @Author: Leo
 * @Date: 2022-02-04 17:46:31
 * @LastEditTime: 2022-08-09 12:09:26
 * @LastEditors: Leo
 */
#pragma once

#ifndef __RAFT_ENGINE_NET_INTERFACE_RAFT_H__
#define __RAFT_ENGINE_NET_INTERFACE_RAFT_H__

extern "C"
{
#include <raft.h>
}

#include <string.h>
#include <utility/tpl.h>

constexpr auto RAFT_BUFLEN = 512;
constexpr auto IP_STR_LEN = strlen("111.111.111.111");

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

using msg_handshake_t = struct
{
  int raft_port;
  int http_port;
  int node_id;
};

using msg_handshake_response_t = struct
{
  int success;
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
  };
  int padding[100];
}; // size 456

using conn_status_e = enum {
  DISCONNECTED,
  CONNECTING,
  CONNECTED,
};

using handshake_state_e = enum {
  HANDSHAKE_FAILURE,
  HANDSHAKE_SUCCESS,
};

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

  struct peer_connection_t *next;
};

#endif // __RAFT_ENGINE_NET_INTERFACE_RAFT_H__