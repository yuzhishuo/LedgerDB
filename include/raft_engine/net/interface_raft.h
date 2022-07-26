/*
 * @Author: Leo
 * @Date: 2022-02-04 17:46:31
 * @LastEditTime: 2022-07-22 09:50:46
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/net/interface_raft.h
 */
#pragma once
#ifndef __RAFT_ENGINE_NET_INTERFACE_RAFT_H__
#define __RAFT_ENGINE_NET_INTERFACE_RAFT_H__

#include <functional>
#include <muduo/net/InetAddress.h>
#include <optional>
#include <utility/tpl.h>
extern "C"
{
#include <raft.h>
}
#define RAFT_BUFLEN 512
#define IP_STR_LEN strlen("111.111.111.111")

typedef struct
{
  uint16_t raft_port;
  uint16_t http_port;
  uint16_t node_id;
  char host[IP_STR_LEN];
} entry_cfg_change_t;

typedef enum
{
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
} peer_message_type_e;

typedef struct
{
  int raft_port;
  int http_port;
  int node_id;
} msg_handshake_t;

typedef struct
{

  int success;
  int leader_port;
  int http_port;
  /* my Raft node ID.
   * Sometimes we don't know who we did the handshake with */
  int node_id;
  char leader_host[IP_STR_LEN];
} msg_handshake_response_t;

typedef struct
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
} msg_t; // size 456

typedef enum
{
  DISCONNECTED,
  CONNECTING,
  CONNECTED,
} conn_status_e;

typedef enum
{
  HANDSHAKE_FAILURE,
  HANDSHAKE_SUCCESS,
} handshake_state_e;

struct peer_connection_t
{

  /* peer's address */
  muduo::net::InetAddress addr;

  int http_port, raft_port;

  /* tell if we need to connect or not */
  conn_status_e connection_status;

  /* gather TPL message */
  tpl_gather_t *gt;

  /* peer's raft node_idx */
  raft_node_t *node;

  /* number of entries currently expected.
   * this counts down as we consume entries */
  int n_expected_entries;

  /* remember most recent append entries msg, we refer to this msg when we
   * finish reading the log entries.
   * used in tandem with n_expected_entries */
  msg_t ae;

  muduo::net::TcpConnectionPtr peer;

  peer_connection_t *next;
};

#endif // __RAFT_ENGINE_NET_INTERFACE_RAFT_H__