#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>

#include "User.h"

enum class LEDGER_ROLE : uint8_t
{
    UNKION,
    OWNER,
    REGULATOR,
    COMMON,
    READONLY
};

class Ledger
{
public:
    Ledger(const std::string &name, std::shared_ptr<User> &owner)
        : name_(name), id_{Ledger::GeneratorId()}, owner_{owner} {}

    // template <typename T>
    // Ledger()
    // {
    // }

    void setOwner(std::shared_ptr<User> &owner)
    {
        owner_ = owner;
    }

    std::shared_ptr<User> owner() const
    {
        if (owner_.expired())
        {
            // TODO: shouldn't nullptr, hava to hava a owner
            return nullptr;
        }
        return owner_.lock();
    }

    LEDGER_ROLE GetRoleByUserName(const std::string &name) const
    {
        if (isOwner(name))
        {
            return LEDGER_ROLE::OWNER;
        }
        else if (isRegulator(name))
        {
            return LEDGER_ROLE::REGULATOR;
        }
        else if (isReadOnly(name))
        {
            return LEDGER_ROLE::READONLY;
        }
        else if (isCommon(name))
        {
            return LEDGER_ROLE::COMMON;
        }
        else
        {
            return LEDGER_ROLE::UNKION;
        }
    }

    bool isOwner(const std::string &name) const
    {
        if (owner_.expired())
        {
            // TODO: shouldn't nullptr, hava to hava a owner
            return false;
        }
        return owner_.lock()->name() == name;
    }

    bool isCommon(const std::string &name) const
    {
        return true;
    }

    bool isRegulator(const std::string &name) const
    {
        return true;
    }

    bool isReadOnly(const std::string &name) const
    {
        return true;
    }

    std::string name() const
    {
        return name_;
    }

    uint64_t id() const
    {
        return id_;
    }

public:
    static uint64_t GeneratorId()
    {
        static uint64_t id = 0;
        return ++id;
    }
    static auto &Ledgers()
    {
        static std::map<std::string, std::shared_ptr<Ledger>> ledgers;
        return ledgers;
    }

private:
    std::string name_;
    uint64_t id_;

    std::weak_ptr<User> owner_;
};

// auto genesis_ledger = Ledger("Genesis", User::admin);

// std::map<std::string, Ledger> ledegers = {
//     {genesis_ledger.name(), genesis_ledger},
// };

// bool CreateLedger(const std::string &ledger_name, std::shared_ptr<User> &owner)
// {
//     if (ledegers.find(ledger_name) != ledegers.end())
//     {
//         return false;
//     }
//     ledegers[ledger_name] = Ledger(ledger_name, owner);
//     return true;
// }
