/*
 * @Author: Leo
 * @Date: 2022-08-12 12:54:23
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-23 03:40:47
 */
#ifndef LEDGER_YUZHI_COMMON_ID_GENERATOR_H
#define LEDGER_YUZHI_COMMON_ID_GENERATOR_H

#include <interfaces/IUnique.h>
#include <memory>
#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction_db.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace yuzhi::common
{
class IdGenerator : public IUnique<uint64_t>
{
public:
  inline IdGenerator(rocksdb::TransactionDB *db) : db_(db)
  {
    assert(db_);
    auto txn = db->BeginTransaction(rocksdb::WriteOptions());
    std::string id = "0";
    if (auto val = txn->GetForUpdate(rocksdb::ReadOptions(), "generatorid", &id); val.IsNotFound())
    {
      txn->Put("generatorid", std::to_string(0));
    }
    else
    {
      for (auto const c : id)
      {
        if (c < '0' && c > '9')
        {
          SPDLOG_ERROR("Invalid character in string: %s", id);
          txn->Put("generatorid", std::to_string(0));
          break;
        }
      }
    }
    if (auto status = txn->Commit(); !status.ok())
    {
      SPDLOG_ERROR("Failed to commit transaction: %s", status.getState());
    }
  };
  ~IdGenerator() override = default;

  inline uint64_t getUnique() const override
  {
    auto transaction = db_->BeginTransaction(rocksdb::WriteOptions{}, rocksdb::TransactionOptions{});

    std::string id;
    transaction->GetForUpdate(rocksdb::ReadOptions{}, "generatorid", &id);

    uint64_t id_ = atoll(id.c_str());
    id_++;
    transaction->Put("generatorid", std::to_string(id_));
    transaction->Commit();
    db_->EndTrace();
    return id_;
  }

private:
  rocksdb::TransactionDB *db_;
};
} // namespace yuzhi::common

#endif // LEDGER_YUZHI_COMMON_ID_GENERATOR_H