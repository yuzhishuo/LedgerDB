/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-24 01:36:32
 */
#include <algorithm>
#include <cassert>
#include <memory>

#include <ledger_engine.pb.h>

#include "Ledgers.h"
#include "Users.h"
using namespace yuzhi;

Ledgers::Ledgers() : ledgers_(), impl_(kLedgerStoreName), users_{impl_.getRawDBPtr()} {}

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

  ledger_engine::Ledger mono_ledger;
  mono_ledger.set_name(name);
  impl_.createLedger(mono_ledger);
  auto ledger = std::make_shared<Ledger>(dynamic_cast<meta::IAccountAtrribute *>(&users_), std::move(mono_ledger));
  ledgers_.insert(std::make_pair(name, ledger));

  return ledger;
}

Ledgers::~Ledgers() { users_.dispose(); }

bool Ledgers::removeLedger(const std::shared_ptr<Ledger> &ledger) { return removeLedger(ledger->name()); }

bool Ledgers::removeLedger(const std::string &ledger_name)
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

bool Ledgers::hasLedger(const std::string &name) const { return ledgers_.count(name) && impl_.hasLedger(name); }

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