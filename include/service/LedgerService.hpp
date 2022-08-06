/*
 * @Author: Leo
 * @Date: 2022-02-07 15:44:35
 * @LastEditTime: 2022-08-06 14:08:38
 * @LastEditors: Leo
 */

#pragma once
#ifndef YUZHI_SERVICE_Ledger_SERVICE_HPP
#define YUZHI_SERVICE_Ledger_SERVICE_HPP

#include <ledger_engine.grpc.pb.h>
#include <meta/Ledgers.h>
// #include <raft_engine/net/RaftService.hpp>
#include <spdlog/spdlog.h>

namespace yuzhi::service
{

class LedgerService
{
public:
  LedgerService() {}
  virtual ~LedgerService() = default;

public:
  virtual grpc::Status CreateLedger(grpc::ServerContext *context, const ::ledger_engine::CreateLedgerRequest *request,
                                    ::ledger_engine::Response *response)
  {
    SPDLOG_INFO("create leadger request: {}", request->DebugString());

    return ::grpc::Status::OK;
  }

private:
};
} // namespace yuzhi::service

#endif // YUZHI_SERVICE_ledger_SERVICE_HPP