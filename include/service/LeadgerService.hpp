/*
 * @Author: Leo
 * @Date: 2022-02-07 15:44:35
 * @LastEditTime: 2022-02-07 17:38:45
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/service/LeadgerService.hpp
 */

#pragma once
#ifndef YUZHI_SERVICE_LEADGER_SERVICE_HPP
#define YUZHI_SERVICE_LEADGER_SERVICE_HPP

#include <Ledgers.h>
#include <ledger_engine.grpc.pb.h>
namespace yuzhi::service {
class LedgerService : public ledger_engine::LedgerService::Service {

public:
  virtual ::grpc::Status
  CreateLedger(::grpc::ServerContext *context,
               const ::ledger_engine::CreateLedgerRequest *request,
               ::ledger_engine::Response *response) {

    auto &leadgers = Ledgers::getInstance();

    if (auto new_ledger = leadgers.createLedger(request->ledgername(),
                                                ("request->owner()", "admin"));
        !new_ledger) {
      response->set_success(false);
      response->set_message("create ledger failed");
      return ::grpc::Status(::grpc::StatusCode::ALREADY_EXISTS, "");
    }
    
    response->set_success(true);
    response->set_message("create ledger success");
    return ::grpc::Status::OK;
  }

private:
};
} // namespace yuzhi::service

#endif // YUZHI_SERVICE_LEADGER_SERVICE_HPP