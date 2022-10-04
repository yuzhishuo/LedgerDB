/*
 * @Author: Leo
 * @Date: 2022-08-06 03:15:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-14 15:09:56
 */

#pragma once
#ifndef LEDGER_YUZHI_META_JOURNAL_MIDDLE
#define LEDGER_YUZHI_META_JOURNAL_MIDDLE
#include <interfaces/IMonostate.h>
#include <journal.pb.h>
#include <memory>
#include <rocksdb/utilities/transaction_db.h>
#include <spdlog/spdlog.h>
#include <string>
#include <utility/Hash.h>
#include <utility>

namespace yuzhi::meta
{
class JournalMiddle : public IMonostate<journal_engine::JournalReceipt>
{
public:
  explicit JournalMiddle(std::weak_ptr<rocksdb::TransactionDB> db) : db_{db} {}

  std::optional<common::Error> generate(const journal_engine::JournalRequest &request,
                                        journal_engine::JournalProxying &journalProxying) const
  {
    if (std::string serialized; !request.SerializeToString(&serialized))
    {
      using yuzhi::utility::Hash;
      using yuzhi::utility::HashType;
      Hash<HashType::SHA256> hasher;
      hasher.add(serialized);

      std::string hash;
      if (auto err = hasher.final1(hash); err)
      {
        SPDLOG_ERROR("Failed to generate hash from journal.");
        return err;
      }
      else
      {
        journalProxying.set_request_hash(hash);
      }
    }
    else
    {
      return common::Error::SerializeError();
    }
    return std::nullopt;
  }

  std::optional<common::Error> store(std::string_view key, const journal_engine::JournalProxying &proxying) const
  {
    if (std::string serialized; !proxying.SerializeToString(&serialized))
    {
      if (auto db = db_.lock(); db)
      {
        db->Put(rocksdb::WriteOptions{}, key, serialized);
      }
    }
    else
    {
      return common::Error::SerializeError();
    }
    return std::nullopt;
  }

private:
  std::optional<common::Error> store(const std::string_view key,
                                     const journal_engine::JournalReceipt &journalReceipt) const
  {
    if (std::string serialized; !journalReceipt.SerializeToString(&serialized))
    {
      if (auto db = db_.lock(); db)
      {
        db->Put(rocksdb::WriteOptions{}, key, serialized);
      }
    }
    else
    {
      return common::Error::SerializeError();
    }
    return std::nullopt;
  }

private:
  Monostate<journal_engine::JournalReceipt> journal_;
  std::weak_ptr<rocksdb::TransactionDB> db_;
};

} // namespace yuzhi::meta

#endif // LEDGER_YUZHI_META_JOURNAL_MIDDLE