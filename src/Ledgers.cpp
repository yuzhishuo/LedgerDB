#include <cassert>
#include <algorithm>

#include "Ledgers.h"

std::shared_ptr<Ledger> Ledgers::createLedger(const std::string &name, const std::string &owner)
{
    if (hasLedger(name))
    {
        return nullptr;
    }

    auto ledger = std::make_shared<Ledger>(name, owner);
    ledgers_.insert(std::make_pair(name, ledger));
    return ledger;
}

bool Ledgers::removeLedger(const std::shared_ptr<Ledger> &ledger)
{
    auto it = ledgers_.find(ledger->name());
    if (it == ledgers_.end())
    {
        return false;
    }
    
    ledgers_.erase(it);
    // assert(it->second.unique());
    return true;
}

bool Ledgers::hasLedger(const std::string &name) const
{
    return ledgers_.count(name) > 0;
}

std::shared_ptr<Ledger> Ledgers::getLedger(const std::string &name) const
{
    if (auto it = ledgers_.find(name); it != ledgers_.end())
    {
        return it->second;
    }
    return nullptr;
}