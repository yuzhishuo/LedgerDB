/*
 * @Author: Leo
 * @Date: 2022-02-07 15:44:35
 * @LastEditTime: 2022-09-17 12:22:06
 * @LastEditors: Leo
 */

#pragma once
#ifndef YUZHI_SERVICE_Ledger_SERVICE_HPP
#define YUZHI_SERVICE_Ledger_SERVICE_HPP

#include <meta/LedgerFactory.h>
#include <raft_engine/RaftEngine.h>
#include <spdlog/spdlog.h>

#include <rocksdb/utilities/transaction_db.h>

namespace yuzhi::service
{

class LedgerService
{
public:
  LedgerService() {}
  virtual ~LedgerService() = default;

public:
  virtual bool CreateLedger()
  {
    return true;
  }
};
} // namespace yuzhi::service

#endif // YUZHI_SERVICE_ledger_SERVICE_HPP