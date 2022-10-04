/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-17 12:00:23
 */
#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <common/TimeWheel.h>
#include <interfaces/IStorable.h>
#include <ledger_engine.pb.h>
#include <meta/Ledger.h>
#include <meta/LedgerFactoryImpl.h>
#include <meta/UserFactory.h>
#include <store/PersistenceStore.h>

namespace yuzhi
{
class User;

constexpr auto kLedgerStoreName = "ledgerdb";

class LedgerFactory : public utility::Noncopyable
{
public:
  using Raw = Ledger;
  using Element = std::shared_ptr<Ledger>;
  using UniqueType = std::common_type_t<decltype(((Ledger *)nullptr)->getUnique())>;

public:
  LedgerFactory();
  ~LedgerFactory();

public:
  static LedgerFactory &Instance()
  {
    static LedgerFactory instance;
    return instance;
  }

public:
  std::shared_ptr<Ledger> createLedger(const std::string &name, const std::string &owner);
  bool removeLedger(const std::string &ledger_name);
  bool removeLedger(const std::shared_ptr<Ledger> &ledger);

  /**
   * @brief remove all ledgers which owner is user
   *
   * @param user ledger owner
   * @return true ledger is removed / false  no ledger is removed
   */
  bool removeLedgerByUser(const std::shared_ptr<User> &user);
  bool hasLedger(const std::string &name) const;

  std::shared_ptr<Ledger> getLedger(const std::string &name) const;

private:
  std::map<std::string, Element, std::less<>> ledgers_;
  common::TimeWheel<Element> time_wheel_;
  LedgerFactoryImpl impl_;
  UserFactory users_;
};
} // namespace yuzhi