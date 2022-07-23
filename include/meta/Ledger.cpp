#include <algorithm>
#include <cassert>

#include "Ledger.h"
#include "Ledgers.h"
#include "User.h"
#include "Users.h"

#include <LedgerEngine.h>
#include <meta/AccountAtrribute.h>

namespace yuzhi
{

std::shared_ptr<LedgerEngine> Ledger::engine() { return std::make_shared<LedgerEngine>(shared_from_this()); }

Ledger::Ledger(meta::IAccountAtrribute* attribute, const std::string &name, const std::string &owner) : ledger_{}, attribute_(attribute)
{
  ledger_->set_owner(owner);
  ledger_->set_name(name);
  ledger_->set_id(Ledger::generatorId());
}

Ledger::Ledger(meta::IAccountAtrribute* attribute, ledger_engine::Ledger &&ledger_inner) : ledger_(std::move(ledger_inner)), attribute_(attribute) {}

// void Ledger::setOwner(std::shared_ptr<User> &owner)
// {
//     owner_ = owner;
// }

// std::shared_ptr<User> Ledger::owner() const
// {
//     if (owner_.expired())
//     {
//         const auto &onwer_name =
//         std::get<ledger_engine::Ledger>(ledger_).onwer();
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

std::optional<common::Error> Ledger::addRegulator(const std::string &name)
{
  return attribute_->addRegulator(GetUnique(), name);
}

std::optional<common::Error> Ledger::addCommon(const std::string &name)
{ 
  return attribute_->addCommon(GetUnique(), name);
}
std::optional<common::Error> Ledger::addReadOnly(const std::string &name)
{
  return attribute_->addReadOnly(GetUnique(), name);
}

std::optional<common::Error> Ledger::removeCommon(const std::string &name)
{
  return attribute_->removeCommon(GetUnique(), name);
}

std::optional<common::Error> Ledger::removeRegulator(const std::string &name)
{
  return attribute_->removeRegulator(GetUnique(), name);
}

std::optional<common::Error> Ledger::removeReadOnly(const std::string &name)
{
  return attribute_->removeReadOnly(GetUnique(), name);
}

LEDGER_ROLE Ledger::getRoleByUserName(const std::string &name) const
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
  return attribute_->isOwner(GetUnique(), name);
}

std::shared_ptr<User> Ledger::Onwer() const
{
  return attribute_->Onwer(GetUnique());
}

bool Ledger::isCommon(const std::string &name) const
{
  return attribute_->isCommon(GetUnique(), name);
}

bool Ledger::isRegulator(const std::string &name) const
{
  return attribute_->isRegulator(GetUnique(), name);
}

bool Ledger::isReadOnly(const std::string &name) const
{
  return attribute_->isReadOnly(GetUnique(), name);
}

const std::string &Ledger::name() const { return ledger_->name(); }

uint64_t Ledger::id() const { return ledger_->id(); }

} // namespace yuzhi