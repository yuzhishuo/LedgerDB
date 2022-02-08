/*
 * @Author: Leo
 * @Date: 2022-01-25 21:35:46
 * @LastEditTime: 2022-02-08 15:17:43
 * @LastEditors: Please set LastEditors
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
#include <string>
extern "C" {
#include <raft.h>
}
#include <config/Config.h>
#include <raft_engine/RaftEngine.h>
#include <service/LeadgerService.hpp>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace spd = spdlog;
using namespace std;
using namespace grpc;
namespace yuzhi {
class Leadger final : public IConfigurable {

public:
  Leadger() = default;
  virtual ~Leadger() {}
  virtual const char *Field() const override { return "ledger"; }

  void start() {

    auto &config = yuzhi::Config::Instance();
    auto prot = config.get<int>(this, "server_port");
    // grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    yuzhi::service::LedgerService leadgerService;
    std::string service_address = "localhost:" + to_string(prot);
    spdlog::info("start leadger service at {}", service_address);
    builder.AddListeningPort(service_address, InsecureServerCredentials());
    // builder.set_health_check_service(new HealthCheckServiceImpl());
    builder.RegisterService(&leadgerService);
    auto server = builder.BuildAndStart();

    server->Wait();
  }
};
} // namespace yuzhi

int main(int argc, char **argv) {

  yuzhi::Leadger leadger;
  leadger.start();

  return 0;
}
