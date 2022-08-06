/*
 * @Author: Leo
 * @Date: 2022-08-06 03:15:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-06 13:40:55
 */

#pragma once
#ifndef LEDGER_YUZHI_META_JOURNAL_MIDDLE
#define LEDGER_YUZHI_META_JOURNAL_MIDDLE
#include "interfaces/IMonostate.h"
#include <journal.pb.h>
#include <openssl/ssl.h>
#include <spdlog/spdlog.h>
#include <string>
#include <utility/Hash.h>

namespace yuzhi::meta
{

class JournalMiddle : public IMonostate<journal_engine::JournalReceipt>
{
public:
  // TODO: implement
  std::optional<common::Error> generate(journal_engine::JournalRequest request,
                                        journal_engine::JournalProxying &journalProxying) const
  {

    if (std::string serialized; !request.SerializeToString(&serialized))
    {
      yuzhi::utility::Hash<yuzhi::utility::HashType::SHA256> hash;
      hash.add(serialized);

      if (auto [err, dig] = hash.final(); err)
      {
        SPDLOG_ERROR("Failed to generate hash from journal ");
        return err;
      }
      else
      {
        journalProxying.set_request_hash(dig);
      }
    }
    return std::nullopt;
  }
  void store(journal_engine::JournalProxying);

private:
  Monostate<journal_engine::JournalReceipt> journal_;
};

} // namespace yuzhi::meta

#endif // LEDGER_YUZHI_META_JOURNAL_MIDDLE