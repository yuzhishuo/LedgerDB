/*
 * @Author: Leo
 * @Date: 2022-01-25 21:35:46
 * @LastEditTime: 2022-08-06 14:01:37
 * @LastEditors: Leo
 * @FilePath: /LedgerDB/main.cpp
 */

#include <iostream>
#include <memory>

#include <rocksdb/db.h>

#include "meta/User.h"
#include <spdlog/spdlog.h>
// #include "rbac/AuthorityCertification.h"
#include "store/PersistenceStore.h"

// tmp
#include "interfaces/IUnique.h"
#include "meta/Ledgers.h"
#include "meta/Users.h"
#include <string>

#include <config/Config.h>
// #include <raft_engine/RaftEngine.h>
#include <service/LedgerService.hpp>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace spd = spdlog;
using namespace std;
using namespace grpc;
namespace yuzhi
{
class LedgerDB final : public IConfigurable
{

public:
  const char *Field() const override { return "ledger"; }

  int start() const
  {
    auto &config = yuzhi::Config::Instance();
    auto prot = config.get<int>(this, "server_port");
    // using yuzhi::raft_engine::net::RaftService;
    // RaftService::Instance();
    SPDLOG_INFO("start ledger service at localhost:{}", prot);
    return 0;
  }
};
} // namespace yuzhi

int main(int argc, char **argv) { return yuzhi::LedgerDB{}.start(); }
