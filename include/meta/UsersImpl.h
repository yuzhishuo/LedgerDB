/*
 * @Author: Leo
 * @Date: 2022-07-20 12:21:39
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-23 16:34:03
 */
#pragma once

#include "meta/AccountAtrribute.h"
#include <common/Error.h>
#include <interfaces/IDisposable.h>
#include <memory>
#include <optional>
#include <string>
#include <user_engine.pb.h>

namespace rocksdb
{
class DB;
class ColumnFamilyHandle;
class OptimisticTransactionDB;
} // namespace rocksdb

namespace yuzhi
{
class User;
}
namespace yuzhi::meta
{

class UsersImpl final : public interface::IDisposable, meta::IAccountAtrribute
{
public:
  UsersImpl(std::weak_ptr<rocksdb::DB> db);
  ~UsersImpl() override;

public:
  // create user object
  std::optional<common::Error> createUser(const std::string &user_name, const std::string &ledger_name) const;

  // delete user object
  std::optional<common::Error> deleteUser(const std::string &ledger_name, const std::string &user_name) const;

  void dispose() override;

public:
  bool isOwner(const std::string &ledger_name, const std::string &name) const override;
  bool isCommon(const std::string &ledger_name, const std::string &name) const override;
  bool isRegulator(const std::string &ledger_name, const std::string &name) const override;
  bool isReadOnly(const std::string &ledger_name, const std::string &name) const override;

  std::shared_ptr<User> Onwer(const std::string &ledger_name) const override;

  std::optional<common::Error> addCommon(const std::string &ledger_name, const std::string &name) override;
  std::optional<common::Error> addRegulator(const std::string &ledger_name, const std::string &name) override;
  std::optional<common::Error> addReadOnly(const std::string &ledger_name, const std::string &name) override;

  std::optional<common::Error> removeCommon(const std::string &ledger_name, const std::string &name) override;
  std::optional<common::Error> removeRegulator(const std::string &ledger_name, const std::string &name) override;
  std::optional<common::Error> removeReadOnly(const std::string &ledger_name, const std::string &name) override;

private:
  std::shared_ptr<rocksdb::OptimisticTransactionDB> captureOptimisticTransactionDB() const;
  std::optional<user_engine::User> captureUser(const std::string &ledger_name, const std::string &user_name) const;

private:
  std::weak_ptr<rocksdb::DB> db_;
  rocksdb::ColumnFamilyHandle *cf_handle_;
}; // namespace yuzhi::meta

} // namespace yuzhi::meta