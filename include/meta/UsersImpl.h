/*
 * @Author: Leo
 * @Date: 2022-07-20 12:21:39
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:16:51
 */
#pragma once

#include <common/Error.h>
#include <interfaces/IDisposable.h>
#include <memory>
#include <optional>
#include <string>

namespace rocksdb
{
class DB;
class ColumnFamilyHandle;
} // namespace rocksdb

namespace yuzhi::meta
{

class UsersImpl final : public interface::IDisposable
{
public:
  UsersImpl(std::weak_ptr<rocksdb::DB> db);

  ~UsersImpl() override;

public:
  // create user object
  std::optional<common::Error> createUser(const std::string &user_name,
                                          const std::string &ledger_name) const;

  // delete user object
  std::optional<common::Error> deleteUser(const std::string &ledger_name,
                                          const std::string &user_name) const;

  void dispose() override;

private:
  std::weak_ptr<rocksdb::DB> db_;
  rocksdb::ColumnFamilyHandle *cf_handle_;
}; // namespace yuzhi::meta

} // namespace yuzhi::meta