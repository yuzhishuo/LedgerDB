/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-20 04:46:54
 */
#pragma once
#include <common/Error.h>
#include <optional>
#include <rocksdb/db.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>
#include <rocksdb/utilities/transaction_db.h>

#include <string>

#include <ledger_engine.pb.h>
#include <user_engine.pb.h>

namespace yuzhi {
  class LedgersImpl {
public:
    LedgersImpl(const std::string &db_name) {

      using namespace rocksdb;
      Options options;
      // open DB with three column families
      std::vector<rocksdb::ColumnFamilyDescriptor> column_families;
      // have to open default column family
      column_families.push_back(
              rocksdb::ColumnFamilyDescriptor(rocksdb::kDefaultColumnFamilyName, rocksdb::ColumnFamilyOptions()));
      // open the new column families
      column_families.push_back({"ledger_db_{ledger_name}", ColumnFamilyOptions()});
      column_families.push_back(
              ColumnFamilyDescriptor("ledger_db_{ledger_name}_user_{user_name}", ColumnFamilyOptions()));

      OptimisticTransactionDB::Open(options, db_name, column_families,
                                    &handles, &txn_db);
    }
    ~LedgersImpl() = default;

public:
    /*
        create ledger with name and owner
        create user 
    */
    // createLedger
    std::optional<Error> createLedger(ledger_engine::Ledger &mono_ledger) {
      using namespace rocksdb;
      WriteOptions write_options;
      ReadOptions read_options;
      OptimisticTransactionOptions txn_options;
      txn_options.set_snapshot = true;
      auto *txn = txn_db->BeginTransaction(write_options, txn_options);
      std::string val;
      Slice key{"ledger_db_" + mono_ledger.name()};

      if (
              auto status = txn->Get(read_options, handles[1], key, &val);
              status.ok()) {
        return Error::RepeatKey();
      }

      if (
              auto status = txn->GetForUpdate(read_options, handles[1], "current_ledger_id", &val);
              !status.ok()) {
        return Error::InternalError(
                "get current_ledger_id failed: " + status.ToString());
      }

      auto current_ledger_id = std::stoi(val);

      if (
              auto status = txn->Put(
                      handles[1], "current_ledger_id",
                      std::to_string(current_ledger_id + 1));
              !status.ok()) {

        return Error::InternalError(
                "put current_ledger_id failed: " + status.ToString());
      }

      mono_ledger.SerializeToString(&val);
      if (
              auto status = txn->Put(handles[1], "ledger_db_" + mono_ledger.name(), val);
              !status.ok()) {

        return Error::InternalError(
                "put ledger_name failed: " + status.ToString());
      }

      if (
              auto status = txn->Put(
                      handles[2],
                      "ledger_db_user_" + mono_ledger.name() + "__id",
                      std::to_string(0));
              !status.ok()) {

        return Error::InternalError(
                "put ledger_db_user_" + mono_ledger.name() + "__id" + " failed: " + status.ToString());
      }

      if (
              auto status = txn->Commit();
              !status.ok()) {

        return Error::InternalError(
                "commit failed: " + status.ToString());
      }

      return std::nullopt;
    }
    /*
        delete user
        delete ledger
    */
    // deleteLedger
    std::optional<Error> deleteLedger(const std::string &ledger_name) {
      using namespace rocksdb;
      WriteOptions write_options;
      ReadOptions read_options;
      OptimisticTransactionOptions txn_options;
      txn_options.set_snapshot = true;
      Transaction *txn = txn_db->BeginTransaction(write_options, txn_options);

      std::string val;
      if (
              auto status = txn->GetForUpdate(read_options, handles[1], "ledger_db_" + ledger_name, &val);
              !status.ok()) {

        return Error::InternalError(
                "get ledger_db_" + ledger_name + " failed: " + status.ToString());
      }
      ledger_engine::Ledger mono_ledger;
      mono_ledger.ParseFromString(val);
      mono_ledger.set_is_deleted(true);

      mono_ledger.SerializeToString(&val);
      txn->Put(handles[1], "ledger_db_" + mono_ledger.name(), val);
      txn->Commit();
      return std::nullopt;
    }

private:
    rocksdb::OptimisticTransactionDB *txn_db;
    std::vector<rocksdb::ColumnFamilyHandle *> handles;
  };
}// namespace yuzhi