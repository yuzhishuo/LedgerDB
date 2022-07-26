/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-23 03:14:32
 */
#pragma once
#include "meta/Constant.h"
#include <common/Error.h>
#include <memory>
#include <optional>
#include <rocksdb/db.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>
#include <rocksdb/utilities/transaction_db.h>
#include <string>

#include <ledger_engine.pb.h>
#include <user_engine.pb.h>

namespace yuzhi
{
class LedgersImpl
{
public:
  LedgersImpl(const std::string &db_name);
  ~LedgersImpl() = default;

public:
  std::weak_ptr<rocksdb::DB> getRawDBPtr() const;

public:
  // createLedger
  std::optional<common::Error> createLedger(ledger_engine::Ledger &mono_ledger);

  bool hasLedger(const std::string &ledger_name) const;

  // deleteLedger
  std::optional<common::Error> deleteLedger(const std::string &ledger_name);

private:
  std::shared_ptr<rocksdb::OptimisticTransactionDB> txn_db;
  std::vector<rocksdb::ColumnFamilyHandle *> handles;
};
} // namespace yuzhi