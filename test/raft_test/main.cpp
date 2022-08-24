/*
 * @Author: Leo
 * @Date: 2022-08-16 05:40:16
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-24 05:03:17
 */

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <raft_engine/net1/RaftServer.h>
#include <rocksdb/utilities/transaction_db.h>
#include <spdlog/spdlog.h>
#include <thread>
using namespace yuzhi::raft_engine;
using namespace rocksdb;
int main()
{
  TransactionDB *txn_db;
  Options options;
  options.create_if_missing = true;

  Status s = TransactionDB::Open(options, {}, "/tmp/raftst", &txn_db);
  spdlog::set_level(spdlog::level::level_enum::trace);
  spdlog::set_pattern("[luluyuzhi:ledgerdb][%Y-%m-%d %H:%M:%S.%e] [%@] [Tid %t] [%l]- %v");
  RaftServer server{txn_db, 3888, 3999};
  server.sync_fire->setCallback([](yuzhi::common::Subscription, int i) { spdlog::info("sync_fire callback {}", i); });

  std::thread slave{[&txn_db] {
    address_t cluster_addr;
    memcpy(cluster_addr.host, "127.0.0.1", sizeof("127.0.0.1"));
    cluster_addr.port = 3888;
    RaftServer slave{txn_db, 3388, 3999, cluster_addr};

    std::this_thread::sleep_for(std::chrono::seconds(3));
    slave.start();
  }};

  std::thread slave1{[&txn_db] {
    address_t cluster_addr;
    memcpy(cluster_addr.host, "127.0.0.1", sizeof("127.0.0.1"));
    cluster_addr.port = 3888;
    RaftServer slave{txn_db, 3377, 3999, cluster_addr};

    std::this_thread::sleep_for(std::chrono::seconds(3));
    slave.start();
  }};

  std::thread async{[&server]() {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    auto entry = yuzhi::Monostate<raft::Entry>{};
    entry->set_data("12321312");
    server.sync(entry);
  }};
  slave.detach();
  slave1.detach();
  server.start();
  return 0;
}