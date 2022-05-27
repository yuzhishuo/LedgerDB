/*
 * @Author: Leo
 * @Date: 2022-02-07 15:44:35
 * @LastEditTime: 2022-03-12 14:31:04
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/service/ledgerService.hpp
 */

#pragma once
#ifndef YUZHI_SERVICE_Ledger_SERVICE_HPP
#define YUZHI_SERVICE_Ledger_SERVICE_HPP

#include <ledger_engine.grpc.pb.h>
#include <meta/Ledgers.h>
#include <raft_engine/net/RaftService.hpp>
#include <spdlog/spdlog.h>

namespace yuzhi::service {

class LedgerService : public ledger_engine::LedgerService::Service {

public:
  virtual ::grpc::Status
  CreateLedger(::grpc::ServerContext *context,
               const ::ledger_engine::CreateLedgerRequest *request,
               ::ledger_engine::Response *response) {
    // grpc debug string contian the '\n'
    SPDLOG_INFO("create leadger request: {}", request->DebugString());
    auto &ledgers = Ledgers::getInstance();
    auto &raft = raft_engine::net::RaftService::Instance();

    if (auto e = raft.Save("create_ledger", request->ledgername()); !e) {
      return ::grpc::Status(::grpc::StatusCode::INTERNAL,
                            "create ledger failed");
    }

    if (auto new_ledger = ledgers.createLedger(request->ledgername(),
                                               ("request->owner()", "admin"));
        !new_ledger) {
      response->set_success(false);
      response->set_message("create ledger failed");
      SPDLOG_INFO("create {} ledger failed", request->ledgername());

      return ::grpc::Status(::grpc::StatusCode::ALREADY_EXISTS, "");
    }

    response->set_success(true);
    response->set_message("create ledger success");
    SPDLOG_INFO("create {} ledger success", request->ledgername());
    return ::grpc::Status::OK;
  }

  virtual ~LedgerService() = default;

private:
};
} // namespace yuzhi::service

#endif // YUZHI_SERVICE_ledger_SERVICE_HPP