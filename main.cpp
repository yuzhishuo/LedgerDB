/*
 * @Author: Leo
 * @Date: 2022-01-25 21:35:46
 * @LastEditTime: 2022-02-02 01:16:31
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /LedgerDB/main.cpp
 */
#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON

#include <iostream>
#include <memory>

#include <rocksdb/db.h>

#include "AuthorityCertification.h"
#include "Spd.h"
#include "User.h"
#include "store/PersistenceStore.h"

// tmp
#include "Ledgers.h"
#include "Users.h"
#include "interfaces/IUnique.h"

extern "C" {
#include <raft.h>
}
#include <config/Config.h>
#include <raft_engine/RaftEngine.h>

// tmp
namespace spd = spdlog;
using namespace grpc;
int main(int argc, char **argv) {
  AuthorityCertification::Instance().Start();
  std::cout << AuthorityCertification::Instance().UserPass("CreateUser",
                                                           "admin")
            << std::endl;

  auto &man = Users::getInstance();
  auto t = man.createUser("fff");

  if (t) {
    if (auto error = man.store(t); error) {
      std::cout << *error << std::endl;
    }
  }

  if (AuthorityCertification::Instance().UserPass("CreateLedger", "fff")) {
    auto &man_l = Ledgers::getInstance();
    auto l = man_l.createLedger("f", t->GetUnique());
    man_l.store(l);

    auto en = l->engine();
  }

  auto e = muduo::net::EventLoop();

  raft_server_t *raft = raft_new();

  auto &config = yuzhi::Config::Instance();

  EventLoop loop;
  uint16_t port = static_cast<uint16_t>(5000);
  InetAddress serverAddr(port);
  yuzhi::raft_engine::RaftEngine server(&loop, serverAddr);
  server.start();
  loop.loop();
  return 0;
}
