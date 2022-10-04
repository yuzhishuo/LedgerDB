#include <algorithm>
#include <cassert>

#include "Ledger.h"
#include <meta/LedgerFactory.h>
#include "User.h"
#include "UserFactory.h"

#include <engine/LedgerEngine.h>
#include <meta/AccountAtrribute.h>

namespace yuzhi
{

std::shared_ptr<LedgerEngine> Ledger::engine() { return std::make_shared<LedgerEngine>(shared_from_this()); }

Ledger::Ledger(meta::IAccountAtrribute *attribute, const std::string &name, const std::string &owner)
    : ledger_{}, merkle_engine_{shared_from_this()}, attribute_(attribute)
{
  ledger_->set_owner(owner);
  ledger_->set_name(name);
}

Ledger::Ledger(meta::IAccountAtrribute *attribute, ledger_engine::Ledger &&ledger_inner)
    : ledger_(std::move(ledger_inner)), merkle_engine_{shared_from_this()}, attribute_(attribute)
{
}

void Ledger::dispose()
{
  auto is_remove =  LedgerFactory::Instance().removeLedger(shared_from_this());
  assert(is_remove);
}

std::optional<common::Error> Ledger::addRegulator(const std::string &name)
{
  return attribute_->addRegulator(getUnique(), name);
}

std::optional<common::Error> Ledger::addCommon(const std::string &name)
{
  return attribute_->addCommon(getUnique(), name);
}
std::optional<common::Error> Ledger::addReadOnly(const std::string &name)
{
  return attribute_->addReadOnly(getUnique(), name);
}

std::optional<common::Error> Ledger::removeCommon(const std::string &name)
{
  return attribute_->removeCommon(getUnique(), name);
}

std::optional<common::Error> Ledger::removeRegulator(const std::string &name)
{
  return attribute_->removeRegulator(getUnique(), name);
}

std::optional<common::Error> Ledger::removeReadOnly(const std::string &name)
{
  return attribute_->removeReadOnly(getUnique(), name);
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

bool Ledger::isOwner(const std::string &name) const { return attribute_->isOwner(getUnique(), name); }

std::shared_ptr<User> Ledger::Onwer() const { return attribute_->Onwer(getUnique()); }

bool Ledger::isCommon(const std::string &name) const { return attribute_->isCommon(getUnique(), name); }

bool Ledger::isRegulator(const std::string &name) const { return attribute_->isRegulator(getUnique(), name); }

bool Ledger::isReadOnly(const std::string &name) const { return attribute_->isReadOnly(getUnique(), name); }

const std::string &Ledger::name() const { return ledger_->name(); }

uint64_t Ledger::id() const { return ledger_->id(); }

} // namespace yuzhi