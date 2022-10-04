#include <chrono>
#include <future>
#include <meta/Ledger.h>
#include <string>
#include <thread>

#include <engine/LedgerEngine.h>
#include <merkle_engine.pb.h>
#include <utility/Hash.h>

using std::optional;
using std::chrono::system_clock;
using yuzhi::common::Error;

namespace yuzhi
{

LedgerEngine::LedgerEngine(std::weak_ptr<Ledger> ledger) : ledger_(ledger), merkle_engine_(ledger) {}

// Read  get 你存的东西 ，就要一个参数 jsn
optional<Error> LedgerEngine::Read(const std::string_view &jsn, std::vector<uint8_t> &data) { return std::nullopt; }

// Verify 1001 和 1000 来判断是否是合法的
optional<Error> LedgerEngine::Verify(const std::string &jsn, std::vector<uint8_t> &content)
{
  merkle_engine::LeafMeta leaf_meta;
  if (!leaf_meta.ParseFromString(jsn))
  {
    return Error::SerializeError();
  }

  using yuzhi::utility::Hash;
  using yuzhi::utility::HashType;
  Hash<HashType::SHA256> hasher;

  hasher.add(std::string_view{(char *)content.data(), content.size()});

  std::string hash;
  if (auto e = hasher.final1(hash); e)
  {
    return e;
  }

  // TODO: hash shouldn't be stored in leaf_meta
  // compare hash, if not equal, return error
  if (hash != leaf_meta.hash())
  {
    return Error::HashError();
  }

  return std::nullopt;
}

// Write  set 你存的东西 ，就要一个参数 一个返回值 root hash ,jsn
// 是一个叶子节点的id office
optional<Error> LedgerEngine::Write(const std::vector<uint8_t> &content, std::string &hash)
{
  // begin time
  auto now = system_clock::now();

  auto ledger = ledger_.lock();
  if (!ledger)
  {
    return Error::InvalidLedger();
  }

  auto const &ledger_ref = *ledger;
  // 计算hash

  using yuzhi::utility::Hash;
  using yuzhi::utility::HashType;
  Hash<HashType::SHA256> hasher;

  std::string hash1;

  auto hasher_async = [&]() {
    hasher.add(std::string_view{(char *)content.data(), content.size()});
    if (auto e = hasher.final1(hash1); e)
    {
      return e;
    }
  };

  /**
   * TODO: 将 content 写入到 rocksdb
   */

  // 异步计算 hash
  auto future = std::async(hasher_async);
  // 等待计算完成
  // TODO: 我希望 和存储文件同时完成。
  future.wait();

  // end timeW
  auto end = system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - now);
  SPDLOG_DEBUG("write time: {}", duration.count());

  // 将hash和content存入  Merkle Engine
  if (auto [res, e] = merkle_engine_.Add(hash1); e)
  {
    return e;
  }
  else
  {
    if (auto r = std::string{}; res.SerializeToString(&r))
    {
      swap(r, hash);
      return std::nullopt;
    }
    else
    {
      return Error::SerializeError();
    }
  }
}

// Grant
optional<Error> LedgerEngine::Grant(const std::string &id)
{
  auto ledger = ledger_.lock();
  if (!ledger)
  {
    return Error::InvalidLedger();
  }
  auto &ledger_ref = *ledger;
  ledger_ref.addCommon(id);
  return std::nullopt;
}

// Delete
optional<Error> LedgerEngine::Delete() { return std::nullopt; }

// Purge
optional<Error> LedgerEngine::Purge() { return std::nullopt; }

// Hide
optional<Error> LedgerEngine::Hide() { return std::nullopt; }

} // namespace yuzhi