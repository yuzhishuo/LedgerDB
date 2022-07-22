/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 06:04:42
 */
#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "LedgerStoreCreator.h"
#include "interfaces/IStorable.h"
#include "ledger_engine.pb.h"
#include "meta/Ledger.h"
#include "meta/LedgersImpl.h"
#include "meta/Users.h"
#include "store/PersistenceStore.h"

namespace yuzhi {
class User;

constexpr auto kLedgerStoreName = "ledgerdb";

class Ledgers : public store::IStorable<Ledger> {
public:
  using Raw = Ledger;
  using Element = std::shared_ptr<Ledger>;
  using UniqueType =
      std::common_type_t<decltype(((Ledger *)nullptr)->GetUnique())>;

public:
  Ledgers()
      : ledgers_(), store_creator_{dynamic_cast<store::IStorable<Ledger> *>(
                        new LedgerStoreCreator{"Ledger"})},
        impl_(kLedgerStoreName), store::IStorable<Ledger>(),
        users_{impl_.getRawDBPtr()} {}

  virtual ~Ledgers() = default;

public:
  static Ledgers &getInstance() {
    static Ledgers instance;
    return instance;
  }

public: //  Engine
public: // IStorable
  virtual std::optional<common::Error>
  store(const Element &element) const override {
    if (auto err = store_creator_->store(element); err) {
      return err;
    }

    return std::nullopt;
  }

  virtual Element
  load(const std::shared_ptr<IUnique<UniqueType>> &element) const override {
    return store_creator_->load(element);
  }

public:
  std::shared_ptr<Ledger> createLedger(const std::string &name,
                                       const std::string &owner);
  bool removeLedger(const std::string &ledger_name);
  bool removeLedger(const std::shared_ptr<Ledger> &ledger);
  /**
   * @brief remove all ledgers which owner is user
   *
   * @param user ledger owner
   * @return true ledger is removed
   * @return false  no ledger is removed
   */
  bool removeLedgerByUser(const std::shared_ptr<User> &user);
  bool hasLedger(const std::string &name) const;

  std::shared_ptr<Ledger> getLedger(const std::string &name) const;

private:
  std::map<std::string, Element> ledgers_;
  std::unique_ptr<IStorable<Ledger>> store_creator_;

  LedgersImpl impl_;
  Users users_;
};
} // namespace yuzhi