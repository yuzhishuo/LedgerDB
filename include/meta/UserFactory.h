/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-17 11:21:08
 */
#pragma once

#include <initializer_list>
#include <interfaces/IDisposable.h>
#include <map>
#include <memory>
#include <rocksdb/db.h>
#include <spdlog/spdlog.h>

#include <utility/opeartors.h>

#include "meta/AccountAtrribute.h"
#include "meta/User.h"
#include "meta/UserFactoryImpl.h"

// #define PREDEF_USER "admin"
namespace yuzhi
{
class UserFactory : public interface::IDisposable, public meta::IAccountAtrribute, public utility::Noncopyable
{
public:
  using Raw = User;
  using Element = std::shared_ptr<User>;
  using UniqueType = std::common_type_t<decltype(((User *)nullptr)->getUnique())>;

public:
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

  explicit UserFactory(std::weak_ptr<rocksdb::DB> db);

public:
  std::shared_ptr<User> createUser(const std::string &user_name, const std::string &ledger_name, USER_ROLE role);
  std::shared_ptr<User> getUser(const std::string &name);
  const std::shared_ptr<User> getUser(const std::string &name) const;
  bool removeUser(const std::shared_ptr<User> &user);

private:
  std::map<std::string, std::shared_ptr<User>> users_;
  yuzhi::meta::UserFactoryImpl usersImpl_;
};
} // namespace yuzhi