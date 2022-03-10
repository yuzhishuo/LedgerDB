#include <algorithm>
#include <cassert>

#include "meta/Ledger.h"
#include "meta/Users.h"
#include "meta/User.h"
#include "meta/Ledgers.h"
#include "LedgerEngine.h"

using namespace yuzhi;

std::shared_ptr<LedgerEngine> Ledger::engine()
{
    return std::make_shared<LedgerEngine>(shared_from_this());
}

Ledger::Ledger(const std::string &name, const std::string &owner)
    : ledger_{}
{
    ledger_->set_onwer(owner);
    ledger_->set_name(name);
    ledger_->set_id(Ledger::GeneratorId());
}

Ledger::Ledger(ledger_engine::Ledger &&ledger_inner)
    : ledger_(std::move(ledger_inner))
{
}

// void Ledger::setOwner(std::shared_ptr<User> &owner)
// {
//     owner_ = owner;
// }

// std::shared_ptr<User> Ledger::owner() const
// {
//     if (owner_.expired())
//     {
//         const auto &onwer_name = std::get<ledger_engine::Ledger>(ledger_).onwer();
//         // TODO: User Manager should package a class to manage users.
//         auto users = Users::getInstance();
//         auto onwer = users.getUser(onwer_name);
//         assert(onwer != nullptr);
//         return onwer;
//     }
//     return owner_.lock();
// }

void Ledger::dispose()
{
    auto is_remove = Ledgers::getInstance().removeLedger(shared_from_this());
    assert(is_remove);
}

std::optional<Error> Ledger::addRegulator(const std::string &name)
{
    auto regulators = ledger_->mutable_regulator();
    auto &regulators_ref = *regulators;
    regulators_ref = name;
    return std::nullopt;
}
std::optional<Error> Ledger::addCommon(const std::string &name)
{
    auto commons = ledger_->mutable_commons();
    auto &commons_ref = *commons;
    commons_ref.Add(std::string(name));
    return std::nullopt;
}
std::optional<Error> Ledger::addReadOnly(const std::string &name)
{
    auto read_onlys = ledger_->mutable_readonlys();
    auto read_onlys_ref = *read_onlys;
    read_onlys_ref.Add(std::string(name));
    return std::nullopt;
}

std::optional<Error> Ledger::removeCommon(const std::string &name)
{
    auto commons = ledger_->mutable_commons();
    auto &commons_ref = *commons;
    auto it = std::find(commons_ref.begin(), commons_ref.end(), name);
    if (it == commons_ref.end())
    {
        return std::make_optional<Error>("NOT_FOUND");
    }
    commons_ref.erase(it);
    return std::nullopt;
}

std::optional<Error> Ledger::removeRegulator(const std::string &name)
{
    auto regulators = ledger_->mutable_regulator();
    auto &regulators_ref = *regulators;
    if (regulators_ref != name)
    {
        return std::make_optional<Error>("NOT_FOUND");
    }
    regulators_ref.clear();
    return std::nullopt;
}

std::optional<Error> Ledger::removeReadOnly(const std::string &name)
{
    auto read_onlys = ledger_->mutable_readonlys();
    auto &read_onlys_ref = *read_onlys;
    auto it = std::find(read_onlys_ref.begin(), read_onlys_ref.end(), name);
    if (it == read_onlys_ref.end())
    {
        return std::make_optional<Error>("NOT_FOUND");
    }
    read_onlys_ref.erase(it);
    return std::nullopt;
}

LEDGER_ROLE Ledger::GetRoleByUserName(const std::string &name) const
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

bool Ledger::isOwner(const std::string &name) const
{
    return ledger_->onwer() == name;
    // if (owner_.expired())
    // {
    //     // TODO: shouldn't nullptr, hava to hava a owner
    //     return false;
    // }
    // return owner_.lock()->name() == name;
}

std::shared_ptr<User> Ledger::Onwer() const
{
    // if (owner_.expired())
    // {
    //     const auto &onwer_name = std::get<ledger_engine::Ledger>(ledger_).onwer();
    //     auto &instance = Users::getInstance();
    //     auto onwer = instance.getUser(onwer_name);
    //     // TODO: User Manager should package a class to manage users.
    //     assert(onwer != nullptr);
    //     return onwer;
    // }
    // return owner_.lock();
    return nullptr;
}

bool Ledger::isCommon(const std::string &name) const
{
    const auto &commons = ledger_->commons();
    return std::find_if(commons.begin(), commons.end(), [&name](const auto &common)
                        { return common == name; }) != commons.end();
}

bool Ledger::isRegulator(const std::string &name) const
{
    const auto &regulator = ledger_->regulator();
    return regulator == name;
}

bool Ledger::isReadOnly(const std::string &name) const
{
    const auto &read_onlys = ledger_->readonlys();
    return std::find_if(read_onlys.begin(), read_onlys.end(), [&name](const auto &read_only)
                        { return read_only == name; }) != read_onlys.end();
}

const std::string &Ledger::name() const
{
    return ledger_->name();
}

uint64_t Ledger::id() const
{
    return ledger_->id();
}
