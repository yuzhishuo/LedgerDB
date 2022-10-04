/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-17 10:44:43
 */
#include <algorithm>
#include <cassert>
#include <memory>

#include <ledger_engine.pb.h>

#include "LedgerFactory.h"
#include "UserFactory.h"
using namespace yuzhi;

LedgerFactory::LedgerFactory() : ledgers_{}, impl_{kLedgerStoreName}, users_{impl_.getRawDBPtr()}, time_wheel_{10} {}

std::shared_ptr<Ledger> LedgerFactory::createLedger(const std::string &name, const std::string &owner)
{
  if (hasLedger(name)
#ifdef DEBUG
      && !Users::getInstance().getUser(owner)
#endif // DEBUG
  )
  {
    return nullptr;
  }

  ledger_engine::Ledger mono_ledger;
  mono_ledger.set_name(name);
  impl_.createLedger(mono_ledger);
  auto ledger = std::make_shared<Ledger>(dynamic_cast<meta::IAccountAtrribute *>(&users_), std::move(mono_ledger));
  ledgers_.try_emplace(name, ledger);

  return ledger;
}

LedgerFactory::~LedgerFactory() { users_.dispose(); }

bool LedgerFactory::removeLedger(const std::shared_ptr<Ledger> &ledger) { return removeLedger(ledger->name()); }

bool LedgerFactory::removeLedger(const std::string &ledger_name)
{
  auto it = ledgers_.find(ledger_name);
  if (it == ledgers_.end())
  {
    return false;
  }

  ledgers_.erase(it);
  // assert(it->second.unique());
  impl_.deleteLedger(ledger_name);
  return true;
}

bool LedgerFactory::hasLedger(const std::string &name) const { return ledgers_.count(name) && impl_.hasLedger(name); }

bool LedgerFactory::removeLedgerByUser(const std::shared_ptr<User> &user)
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

  for (const auto &name : remove_ledger_names)
  {
    ledgers_.erase(name);
  }
  return remove_ledger_names.empty();
}

std::shared_ptr<Ledger> LedgerFactory::getLedger(const std::string &name) const
{
  if (auto it = ledgers_.find(name); it != ledgers_.end())
  {
    return it->second;
  }
  return nullptr;
}