#pragma once
#include <string>
#include <vector>
#include <optional>
#include <utility>
#include <memory>
#include <string>

#include <common/Error.h>
#include <merkle/MerkleEngine.h>
class Ledger;

namespace yuzhi
{
    class LedgerEngine
    {
    public:
        LedgerEngine(std::shared_ptr<Ledger> ledger);
        ~LedgerEngine() = default;

    public:
        // Read, get 你存的东西 ，就要一个参数 jsn
        std::pair<std::vector<uint8_t>, std::optional<Error>> Read(const std::string &jsn);
        // Verify 1001 和 1000 来判断是否是合法的
        std::optional<Error> Verify(const std::string &jsn, std::vector<uint8_t>& data);
        // Write  set 你存的东西 ，就要一个参数 一个返回值 root hash ,jsn 是一个叶子节点的id office
        std::pair<std::string, std::optional<Error>> Write(const std::vector<uint8_t>& content);
        // Grant
        std::optional<Error> Grant(const std::string &id);
        // Delete
        std::optional<Error> Delete();
        // Purge
        std::optional<Error> Purge();
        // Hide
        std::optional<Error> Hide();

    private:
        std::weak_ptr<Ledger> ledger_;
        MerkleEngine merkle_engine_;
    };
} // namespace yuzhi