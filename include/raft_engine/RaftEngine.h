/*
 * @Author: Leo
 * @Date: 2022-01-30 13:11:29
 * @LastEditTime: 2022-08-21 00:12:40
 * @LastEditors: Leo
 */
#pragma once
#include <memory>

#include <functional>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <unistd.h>
#include <raft_engine/net1/RaftServer.h>
#include <config/Config.h>

namespace rocksdb
{
 class TransactionDB; 
}
namespace yuzhi::raft_engine
{
class RaftEngine : public IConfigurable
{

public:
  RaftEngine();
  ~RaftEngine() override = default;

  const char *Field() const override { return "raft"; }
  RaftRole role() const { return RaftRole::UNKNOW; }
  int sync(const yuzhi::IMonostate<raft::Entry> &y) { return raft_server_->sync(y); }
  void start();

private:
  void cluster_id_change_cb(common::Subscription, int id);

private:
  int service_id = -1;
  std::unique_ptr<rocksdb::TransactionDB> db_; 
  std::unique_ptr<RaftServer> raft_server_;

public:
  std::shared_ptr<common::Event::Fire> sync_fire = raft_server_->sync_fire;
};
} // namespace yuzhi::raft_engine
