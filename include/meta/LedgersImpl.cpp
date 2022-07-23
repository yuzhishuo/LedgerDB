
#include "meta/LedgersImpl.h"
namespace yuzhi
{

LedgersImpl::LedgersImpl(const std::string &db_name)
{

  using namespace rocksdb;
  Options options;

  std::vector<rocksdb::ColumnFamilyDescriptor> column_families;

  column_families.push_back(
      rocksdb::ColumnFamilyDescriptor(rocksdb::kDefaultColumnFamilyName, rocksdb::ColumnFamilyOptions()));

  column_families.push_back({"ledger_db_{ledger_name}", ColumnFamilyOptions()});
  column_families.push_back(ColumnFamilyDescriptor("ledger_db_{ledger_name}_user_{user_name}", ColumnFamilyOptions()));

  rocksdb::OptimisticTransactionDB *db;
  OptimisticTransactionDB::Open(options, db_name, column_families, &handles, &db);
  // maybe error here
  txn_db.reset(db);
  std::string val;
  if (auto status = txn_db->Get(rocksdb::ReadOptions{}, handles[1], "current_ledger_id", &val); !status.ok())
  {
    txn_db->Put(rocksdb::WriteOptions{}, handles[1], "current_ledger_id", "0");
  }
}

std::weak_ptr<ROCKSDB_NAMESPACE::DB> LedgersImpl::getRawDBPtr() const { return txn_db; }

// createLedger
std::optional<common::Error> LedgersImpl::createLedger(ledger_engine::Ledger &mono_ledger)
{
  using namespace rocksdb;
  WriteOptions write_options;
  ReadOptions read_options;
  OptimisticTransactionOptions txn_options;
  txn_options.set_snapshot = true;
  auto *txn = txn_db->BeginTransaction(write_options, txn_options);
  std::string val;
  Slice key{"ledger_db_" + mono_ledger.name()};

  if (auto status = txn->Get(read_options, handles[1], key, &val); status.ok())
  {
    return common::Error::RepeatKey();
  }

  if (auto status = txn->GetForUpdate(read_options, handles[1], "current_ledger_id", &val); !status.ok())
  {
    return common::Error::InternalError("get current_ledger_id failed: " + status.ToString());
  }

  auto current_ledger_id = std::stoi(val);
  mono_ledger.set_id(current_ledger_id);
  if (auto status = txn->Put(handles[1], "current_ledger_id", std::to_string(current_ledger_id + 1)); !status.ok())
  {

    return common::Error::InternalError("put current_ledger_id failed: " + status.ToString());
  }

  mono_ledger.SerializeToString(&val);
  if (auto status = txn->Put(handles[1], "ledger_db_" + mono_ledger.name(), val); !status.ok())
  {

    return common::Error::InternalError("put ledger_name failed: " + status.ToString());
  }

  if (auto status = txn->Put(handles[2], "ledger_db_user_" + mono_ledger.name() + "__id", std::to_string(0));
      !status.ok())
  {

    return common::Error::InternalError("put ledger_db_user_" + mono_ledger.name() + "__id" +
                                        " failed: " + status.ToString());
  }

  if (auto status = txn->Commit(); !status.ok())
  {

    return common::Error::InternalError("commit failed: " + status.ToString());
  }

  return std::nullopt;
}

bool LedgersImpl::hasLedger(const std::string &ledger_name) const
{
  using namespace rocksdb;
  ReadOptions read_options;
  Slice key{"ledger_db_" + ledger_name};
  std::string val;
  if (auto status = txn_db->Get(read_options, handles[1], key, &val); status.ok())
  {
    return true;
  }
  return false;
}

// deleteLedger
std::optional<common::Error> LedgersImpl::deleteLedger(const std::string &ledger_name)
{
  using namespace rocksdb;
  WriteOptions write_options;
  ReadOptions read_options;
  OptimisticTransactionOptions txn_options;
  txn_options.set_snapshot = true;
  Transaction *txn = txn_db->BeginTransaction(write_options, txn_options);

  std::string val;
  if (auto status = txn->GetForUpdate(read_options, handles[1], "ledger_db_" + ledger_name, &val); !status.ok())
  {
    return common::Error::InternalError("get ledger_db_" + ledger_name + " failed: " + status.ToString());
  }
  ledger_engine::Ledger mono_ledger;
  mono_ledger.ParseFromString(val);
  mono_ledger.set_is_deleted(true);

  mono_ledger.SerializeToString(&val);
  txn->Put(handles[1], "ledger_db_" + mono_ledger.name(), val);
  txn->Commit();
  return std::nullopt;
}
} // namespace yuzhi