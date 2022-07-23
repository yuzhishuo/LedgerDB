/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-23 16:33:37
 */
#pragma once

#include <initializer_list>
#include <interfaces/IDisposable.h>
#include <map>
#include <memory>
#include <rocksdb/db.h>
#include <spdlog/spdlog.h>

#include "UserStoreCreator.h"
#include "meta/AccountAtrribute.h"
#include "meta/User.h"
#include "meta/UsersImpl.h"

// #define PREDEF_USER "admin"
namespace yuzhi
{
class Users : public store::IStorable<User>, public interface::IDisposable, public meta::IAccountAtrribute
{
public:
  using Raw = User;
  using Element = std::shared_ptr<User>;
  using UniqueType = std::common_type_t<decltype(((User *)nullptr)->GetUnique())>;

public:
  static Users &getInstance()
  {
    static Users instance{};
    return instance;
  }

  void dispose() override { usersImpl_.dispose(); }

public: // IAccountAtrribute
  bool isOwner(const std::string &ledger_name, const std::string &name) const override
  {
    return usersImpl_.isOwner(ledger_name, name);
  }
  bool isCommon(const std::string &ledger_name, const std::string &name) const override
  {
    return usersImpl_.isCommon(ledger_name, name);
  }
  bool isRegulator(const std::string &ledger_name, const std::string &name) const override
  {
    return usersImpl_.isRegulator(ledger_name, name);
  }
  bool isReadOnly(const std::string &ledger_name, const std::string &name) const override
  {
    return usersImpl_.isReadOnly(ledger_name, name);
  }

  std::shared_ptr<User> Onwer(const std::string &ledger_name) const override { return usersImpl_.Onwer(ledger_name); }

  std::optional<common::Error> addCommon(const std::string &ledger_name, const std::string &name) override
  {
    return usersImpl_.addCommon(ledger_name, name);
  }
  std::optional<common::Error> addRegulator(const std::string &ledger_name, const std::string &name) override
  {
    return usersImpl_.addRegulator(ledger_name, name);
  }
  
  std::optional<common::Error> addReadOnly(const std::string &ledger_name, const std::string &name) override
  {
    return usersImpl_.addReadOnly(ledger_name, name);
  }

  std::optional<common::Error> removeCommon(const std::string &ledger_name, const std::string &name) override
  {
    return usersImpl_.removeCommon(ledger_name, name);
  }
  std::optional<common::Error> removeRegulator(const std::string &ledger_name, const std::string &name) override
  {
    return usersImpl_.removeRegulator(ledger_name, name);
  }
  std::optional<common::Error> removeReadOnly(const std::string &ledger_name, const std::string &name) override
  {
    return usersImpl_.removeReadOnly(ledger_name, name);
  }

public:
  Users(std::initializer_list<std::pair<std::string, std::shared_ptr<User>>> init);

  Users(std::weak_ptr<rocksdb::DB> db);

public: // IStorable
  virtual std::optional<common::Error> store(const Element &element) const override
  {
    return store_creator_->store(element);
  }

  virtual Element load(const std::shared_ptr<IUnique<UniqueType>> &element) const override
  {
    return store_creator_->load(element);
  }

public:
  std::shared_ptr<User> createUser(const std::string &user_name, const std::string &ledger_name, USER_ROLE role);
  std::shared_ptr<User> getUser(const std::string &name);
  const std::shared_ptr<User> getUser(const std::string &name) const;
  bool removeUser(const std::shared_ptr<User> &user);

private:
  std::map<std::string, std::shared_ptr<User>> users_;
  std::unique_ptr<IStorable<User>> store_creator_;
  yuzhi::meta::UsersImpl usersImpl_;
};
} // namespace yuzhi