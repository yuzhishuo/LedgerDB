#include <cassert>
#include <algorithm>

#include "Ledgers.h"
#include "Users.h"

std::shared_ptr<Ledger> Ledgers::createLedger(const std::string &name, const std::string &owner)
{
    if (hasLedger(name)
#ifdef DEBUG
        && !Users::getInstance().getUser(owner)
#endif // DEBUG
    )
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

bool Ledgers::removeLedgerByUser(const std::shared_ptr<User> &user)
{
    std::vector<std::string> remove_ledger_names;
    for (auto it = ledgers_.begin(); it != ledgers_.end();)
    {
        if (it->second->Onwer() == user)
        {

            remove_ledger_names.push_back(it->first);
        }
        else
        {
            ++it;
        }
    }

    for (auto &name : remove_ledger_names)
    {
        ledgers_.erase(name);
    }
    return remove_ledger_names.empty();
}

std::shared_ptr<Ledger> Ledgers::getLedger(const std::string &name) const
{
    if (auto it = ledgers_.find(name); it != ledgers_.end())
    {
        return it->second;
    }
    return nullptr;
}