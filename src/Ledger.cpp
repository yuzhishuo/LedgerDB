#include "Ledger.h"
#include "User.h"

Ledger::Ledger(const std::string &name, std::shared_ptr<User> &owner)
    : name_(name), id_{Ledger::GeneratorId()}, owner_{owner} {}

void Ledger::setOwner(std::shared_ptr<User> &owner)
{
    owner_ = owner;
}

std::shared_ptr<User> Ledger::owner() const
{
    if (owner_.expired())
    {
        // TODO: shouldn't nullptr, hava to hava a owner
        return nullptr;
    }
    return owner_.lock();
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
    if (owner_.expired())
    {
        // TODO: shouldn't nullptr, hava to hava a owner
        return false;
    }
    return owner_.lock()->name() == name;
}

bool Ledger::isCommon(const std::string &name) const
{
    return true;
}

bool Ledger::isRegulator(const std::string &name) const
{
    return true;
}

bool Ledger::isReadOnly(const std::string &name) const
{
    return true;
}

std::string Ledger::name() const
{
    return name_;
}

uint64_t Ledger::id() const
{
    return id_;
}
