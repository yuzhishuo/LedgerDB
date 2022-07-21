/*
 * @Author: Leo
 * @Date: 2022-07-20 12:21:39
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-21 08:06:40
 */
#pragma once

#include <common/Error.h>

#include <optional>
#include <string>

namespace rocksdb {
  class DB;
  class ColumnFamilyHandle;
}

namespace yuzhi::meta {

class UsersImpl final {
public:
  UsersImpl(std::weak_ptr<rocksdb::DB> db);

  ~UsersImpl();

public:
  void IDistory();

  // create user object
  std::optional<common::Error> createUser(const std::string &user_name,
                                  const std::string &ledger_name);

  // delete user object
  std::optional<common::Error> deleteUser(const std::string &user_name);

private:
  std::weak_ptr<rocksdb::DB> db_;
  rocksdb::ColumnFamilyHandle *cf_handle_;
}; // namespace yuzhi::meta

} // namespace yuzhi::meta