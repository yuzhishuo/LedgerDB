/*
 * @Author: Leo
 * @Date: 2022-01-30 13:11:29
 * @LastEditTime: 2022-07-22 09:09:43
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/RaftEngine.h
 */

#pragma once
#include <memory>

#include <spdlog/spdlog.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <muduo/net/EventLoop.h>

extern "C"
{
#include <raft.h>
}

#include <raft.grpc.pb.h>

#include <config/Config.h>

namespace yuzhi::raft_engine
{

class RaftEngine : private raft::Raft::Service, public IConfigurable
{

public:
  RaftEngine()
  {
    auto raft = raft_new();

    void *connection_user_data = NULL;
    size_t node_id = 0;
    auto peer_is_self = 1;
    raft_add_node(raft, connection_user_data, node_id, peer_is_self);

    auto &config = Config::Instance();
    std::string server_address(config.get<std::string>(this, "server_address"));
    grpc::EnableDefaultHealthCheckService(true);

    muduo::net::EventLoop loop;

    loop.runEvery(1000, [this, &raft]() { raft_periodic(raft, 1000); });

    // grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // builder.set_health_check_service(new HealthCheckServiceImpl());
    builder.RegisterService(this);

    server = std::move(std::unique_ptr<grpc::Server>(builder.BuildAndStart()));
    spdlog::info("Raft engine server listening on {}", server_address);
  }

  virtual const char *Field() const override { return "raft"; }

  void Listen()
  {
    // spdlog::info("Raft engine server listening on {}", server_address);
    server->Wait();
  }

  virtual ~RaftEngine() = default;

public:
  std::unique_ptr<grpc::Server> server;
};
} // namespace yuzhi::raft_engine
