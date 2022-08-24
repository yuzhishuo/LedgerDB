/*
 * @Author: Leo
 * @Date: 2022-08-06 16:36:56
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-22 15:23:54
 */
#include <config/Config.h>
#include <functional>
#include <raft_engine/RaftEngine.h>
#include <rocksdb/utilities/transaction_db.h>
#include <utility/Enum.h>

namespace yuzhi::raft_engine
{

RaftEngine::RaftEngine()
{

  {
    rocksdb::TransactionDB *txn_db;
    rocksdb::Options options;
    options.create_if_missing = true;

    rocksdb::Status s = rocksdb::TransactionDB::Open(options, {}, "/tmp/raftst", &txn_db);
    db_.reset(txn_db);
  }
  
  using std::bind;
  using std::placeholders::_1;
  using std::placeholders::_2;

  auto &config = Config::Instance();

  auto raft_port = config.get<int>(this, "raft_port");
  auto engine_port = config.get<int>(this, "engine_port");
  auto join = config.get<bool>(this, "joinable");

  // magic_enum::enum_cast<IdleState>()
  raft_server_ = std::make_unique<RaftServer>(db_.get(), raft_port, engine_port, IdleState::JOIN);

  raft_server_->id_fire->setCallback(bind(&RaftEngine::cluster_id_change_cb, this, _1, _2));
}

void RaftEngine::cluster_id_change_cb([[maybe_unused]] common::Subscription, int id) { service_id = id; }
// RaftEngine::~RaftEngine() {}

void RaftEngine::start() { raft_server_->start(); }

} // namespace yuzhi::raft_engine