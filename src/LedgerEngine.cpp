#include <chrono>

#include "LedgerEngine.h"

using namespace yuzhi;
using namespace std;

LedgerEngine::LedgerEngine(std::shared_ptr<Ledger> ledger)
    : ledger_(ledger), merkle_engine_(ledger)
{
}

// Read  get 你存的东西 ，就要一个参数 jsn
std::pair<std::vector<uint8_t>, std::optional<Error>> LedgerEngine::Read(const std::string &jsn)
{
    return std::make_pair(std::vector<uint8_t>(), std::optional<Error>());
}

// Verify 1001 和 1000 来判断是否是合法的
std::optional<Error> LedgerEngine::Verify(const std::string &jsn1, const std::string &jsn2)
{
    return std::nullopt;
}

// Write  set 你存的东西 ，就要一个参数 一个返回值 root hash ,jsn 是一个叶子节点的id office
std::optional<Error> LedgerEngine::Write(const std::vector<uint8_t> content)
{
    //begin time
    auto now = chrono::system_clock::now();
    auto ledger = ledger_.lock();

    if (ledger == nullptr)
    {
        return Error::InvalidLedger();
    }

    auto &ledger_ref = *ledger;

    return std::nullopt;
}
// Grant
std::optional<Error> LedgerEngine::Grant(const std::string &id)
{
    return std::nullopt;
}
// Delete
std::optional<Error> LedgerEngine::Delete()
{
    return std::nullopt;
}
// Purge
std::optional<Error> LedgerEngine::Purge()
{
    return std::nullopt;
}
// Hide
std::optional<Error> LedgerEngine::Hide()
{
    return std::nullopt;
}
