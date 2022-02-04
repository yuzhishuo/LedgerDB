/*
 * @Author: Leo
 * @Date: 2022-02-04 17:46:31
 * @LastEditTime: 2022-02-05 00:55:12
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/net/interface_raft.h
 */
#pragma once
#ifndef __RAFT_ENGINE_NET_INTERFACE_RAFT_H__
#define __RAFT_ENGINE_NET_INTERFACE_RAFT_H__

#include <functional>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpServer.h>
#include <optional>

extern "C" {
#include <raft.h>
}
#define RAFT_BUFLEN 512
#define IP_STR_LEN strlen("111.111.111.111")

typedef enum {
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

typedef struct {
  int raft_port;
  int http_port;
  int node_id;
} msg_handshake_t;

typedef struct {

  int success;

  /* leader's Raft port */
  int leader_port;

  /* the responding node's HTTP port */
  int http_port;

  /* my Raft node ID.
   * Sometimes we don't know who we did the handshake with */
  int node_id;

  char leader_host[IP_STR_LEN];
} msg_handshake_response_t;

typedef struct {
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
} msg_t; // size 440

typedef enum {
  DISCONNECTED,
  CONNECTING,
  CONNECTED,
} conn_status_e;

struct peer_connection_t {

  void *self;
  /* peer's address */
  muduo::net::InetAddress addr;

  int http_port, raft_port;

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

  muduo::net::TcpConnectionPtr peer;

  std::unique_ptr<muduo::net::TcpClient> cl;

  peer_connection_t *next;
};

extern "C" {

/** Raft callback for applying an entry to the finite state machine */
int __raft_applylog(raft_server_t *raft, void *udata, raft_entry_t *ety,
                    raft_index_t entry_idx) {

  return -1;
}

int __raft_persist_vote(raft_server_t *raft, void *udata, const int voted_for) {
  return -1;
}

int __raft_persist_term(raft_server_t *raft, void *udata,
                        raft_term_t current_term, raft_node_id_t vote) {
  return -1;
}

int __raft_logentry_offer(raft_server_t *raft, void *udata, raft_entry_t *ety,
                          raft_index_t ety_idx) {

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
  return -1;
}
void __raft_log(raft_server_t *raft, raft_node_t *node, void *udata,
                const char *buf) {

  spdlog::debug("raft: {}", buf);
}
}
#endif // __RAFT_ENGINE_NET_INTERFACE_RAFT_H__