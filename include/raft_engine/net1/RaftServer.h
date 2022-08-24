/*
 * @Author: Leo
 * @Date: 2022-08-06 17:21:29
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-22 15:01:34
 */
#pragma once

#ifndef YUZHI_RAFT_ENGINE_RAFT_SERVER_H_
#define YUZHI_RAFT_ENGINE_RAFT_SERVER_H_

#ifndef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

extern "C"
{
#include <raft.h>
}
#include "uv_helpers.h"
#include <any>
#include <common/Event.h>
#include <common/IdGenerator.h>
#include <interfaces/IMonostate.h>
#include <raft.pb.h>
#include <raft_engine/net1/interface_raft.h>
#include <spdlog/spdlog.h>
#include <stdlib.h> // for random
#include <stdlib.h>
#include <utility/Snow.h>
#include <uv.h>
#include <uv/errno.h>

namespace yuzhi::raft_engine
{

class RaftServer
{
private:
  /* data */
public:
  RaftServer(rocksdb::TransactionDB *db, int raft_port, int engine_port, IdleState state = IdleState::START);
  RaftServer(rocksdb::TransactionDB *db, int raft_port, int engine_port, address_t cluster_address);
  ~RaftServer();

public:
  void leave();
  int sync(const IMonostate<raft::Entry> &mono);
  void start();
  bool isLeader() const;

private:
  void timer_init(RaftServerInterface *raftServiceIniterface) const noexcept;
  void callback_init(RaftServerInterface *raftServiceIniterface) noexcept;

private:
  common::IdGenerator generator_;
  RaftServerInterface interface_;

private:
#pragma region raft call define
  friend int __raft_send_requestvote(raft_server_t *raft, void *udata, raft_node_t *node, msg_requestvote_t *m);
  friend int __raft_send_appendentries(raft_server_t *raft, void *user_data, raft_node_t *node, msg_appendentries_t *m);
  friend int __raft_applylog(raft_server_t *raft, void *user_data, raft_entry_t *entry, raft_index_t entry_idx);
  friend int __raft_persist_vote(raft_server_t *raft, void *udata, raft_node_id_t vote);
  friend int __raft_persist_term(raft_server_t *raft, void *user_data, raft_term_t term, raft_node_id_t vote);
  friend int __raft_logentry_offer(raft_server_t *raft, void *udata, raft_entry_t *ety, raft_index_t entry_idx);
  friend int __raft_logentry_poll(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx);
  friend int __raft_logentry_pop(raft_server_t *raft, void *udata, raft_entry_t *entry, raft_index_t ety_idx);
  friend raft_node_id_t __raft_log_get_node_id(raft_server_t *raft, void *udata, raft_entry_t *entry,
                                               raft_index_t ety_idx);
  /** Non-voting node now has enough logs to be able to vote.
   * Append a finalization cfg log entry. */
  friend int __raft_node_has_sufficient_logs(raft_server_t *raft, void *user_data, raft_node_t *node);
  friend void __raft_log(raft_server_t *raft, raft_node_t *node, void *udata, const char *buf);
  friend int __raft_send_snapshot(raft_server_t *raft, void *user_data, raft_node_t *node);
#pragma endregion raft call define

#pragma region event
private:
  common::Event id_event_{"id_event"};
  common::Event server_state_event_{"server_state_event"};
  common::Event sync_event_{"sync_event"};

public:
  std::shared_ptr<common::Event::Fire> id_fire = id_event_.fire();
  std::shared_ptr<common::Event::Fire> server_state_fire = server_state_event_.fire();
  std::shared_ptr<common::Event::Fire> sync_fire = sync_event_.fire();
#pragma endregion
};
} // namespace yuzhi::raft_engine

#endif