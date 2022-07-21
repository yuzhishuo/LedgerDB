#include <chrono>
#include <string>
#include <thread>
#include <future>
#include <iostream>
#include <meta/Ledger.h>
#include <openssl/sha.h>

#include "LedgerEngine.h"
#include <merkle_engine.pb.h>

using namespace yuzhi;
using namespace std;

LedgerEngine::LedgerEngine(std::shared_ptr<Ledger> ledger)
    : ledger_(ledger), merkle_engine_(ledger)
{
}

// Read  get 你存的东西 ，就要一个参数 jsn
std::pair<std::vector<uint8_t>, std::optional<common::Error>> LedgerEngine::Read(const std::string &jsn)
{
    return std::make_pair(std::vector<uint8_t>(), std::optional<common::Error>());
}

// Verify 1001 和 1000 来判断是否是合法的
std::optional<common::Error> LedgerEngine::Verify(const std::string &jsn, std::vector<uint8_t> &content)
{
    merkle_engine::LeafMeta leaf_meta;
    if (!leaf_meta.ParseFromString(jsn))
    {
        return common::Error::SerializeError();
    }

    // hash data
    uint8_t hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, content.data(), content.size());
    SHA256_Final(hash, &sha256);

    // TODO: hash shouldn't be stored in leaf_meta
    // compare hash, if not equal, return error
    if (memcmp(hash, leaf_meta.hash().data(), SHA256_DIGEST_LENGTH) != 0)
    {
        return common::Error::HashError();
    }

    return std::optional<common::Error>();
}

// Write  set 你存的东西 ，就要一个参数 一个返回值 root hash ,jsn 是一个叶子节点的id office
std::pair<std::string, std::optional<common::Error>> LedgerEngine::Write(const std::vector<uint8_t> &content)
{
    // begin time
    auto now = chrono::system_clock::now();

    auto ledger = ledger_.lock();
    if (!ledger)
    {
        return {{}, common::Error::InvalidLedger()};
    }

    auto &ledger_ref = *ledger;
    // 计算hash
    uint8_t hash[SHA256_DIGEST_LENGTH];
    auto hasher = [&]()
    {
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, content.data(), content.size());
        SHA256_Final(hash, &sha256);
    };

    /**
     * TODO: 将 content 写入到 rocksdb
     */

    // 异步计算 hash
    auto future = std::async(hasher);
    // 等待计算完成
    // TODO: 我希望 和存储文件同时完成。
    future.wait();

    // end timeW
    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - now);
    cout << "write time: " << duration.count() << endl;

    // 将hash和content存入  Merkle Engine
    if (auto [res, e] = merkle_engine_.Add(std::string{hash, hash + SHA256_DIGEST_LENGTH}); e)
    {
        return {{}, e};
    }
    else
    {
        if (auto r = std::string{}; res.SerializeToString(&r))
        {
            return {r, std::nullopt};
        }
        else
        {
            return {{}, common::Error::SerializeError()};
        }
    }
}

// Grant
std::optional<common::Error> LedgerEngine::Grant(const std::string &id)
{
    auto ledger = ledger_.lock();
    if (!ledger)
    {
        return common::Error::InvalidLedger();
    }
    auto &ledger_ref = *ledger;
    ledger_ref.addCommon(id);
    return std::nullopt;
}

// Delete
std::optional<common::Error> LedgerEngine::Delete()
{
    return std::nullopt;
}

// Purge
std::optional<common::Error> LedgerEngine::Purge()
{
    return std::nullopt;
}

// Hide
std::optional<common::Error> LedgerEngine::Hide()
{
    return std::nullopt;
}
