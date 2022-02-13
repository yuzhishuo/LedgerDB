/*
 * @Author: Leo
 * @Date: 2022-02-08 22:59:45
 * @LastEditors: Leo
 * @LastEditTime: 2022-02-09 17:23:57
 */

#pragma once
#ifndef CLI_INCLUDE_SERVICE_LEDGER_SERVICE_HPP_
#define CLI_INCLUDE_SERVICE_LEDGER_SERVICE_HPP_

#include <grpcpp/grpcpp.h>
#include <ledger_engine.grpc.pb.h>
#include <ledger_engine.pb.h>
#include <spdlog/spdlog.h>
#include <string>
namespace yuzhi::cli {

class ledgerClient {
public:
  ledgerClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(ledger_engine::LedgerService::NewStub(channel)){};
  ~ledgerClient() = default;

public:
  bool CreateLedger(std::string name) {
    ledger_engine::CreateLedgerRequest request;
    request.set_ledgername(name);
    ledger_engine::Response response;

    ::grpc::ClientContext context;
    if (auto status = stub_->CreateLedger(&context, request, &response);
        status.ok()) {
      if (response.success()) {
        spdlog::info(response.message());
        return true;
      }
    }
    spdlog::error("CreateLedger failed");
    return false;
  }

private:
  std::shared_ptr<ledger_engine::LedgerService::Stub> stub_;
};

} // namespace yuzhi::cli

#endif // CLI_INCLUDE_SERVICE_LEDGER_SERVICE_HPP_