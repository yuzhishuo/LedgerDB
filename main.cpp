/*
 * @Author: Leo
 * @Date: 2022-01-25 21:35:46
 * @LastEditTime: 2022-02-07 18:19:45
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
#include <service/LeadgerService.hpp>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

// tmp
namespace spd = spdlog;
using namespace grpc;
int main(int argc, char **argv) {

  auto &config = yuzhi::Config::Instance();

  // grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  grpc::ServerBuilder builder;
  yuzhi::service::LedgerService leadgerService;
  builder.AddListeningPort("10080", grpc::InsecureServerCredentials());
  // builder.set_health_check_service(new HealthCheckServiceImpl());
  builder.RegisterService(&leadgerService);
  auto server =
      std::move(std::unique_ptr<grpc::Server>(builder.BuildAndStart()));

  server->Wait();

  return 0;
}
