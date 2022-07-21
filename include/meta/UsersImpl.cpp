/*
 * @Author: Leo
 * @Date: 2022-07-21 07:36:48
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-21 08:54:15
 */
#include "meta/Constant.h"
#include "meta/UsersImpl.h"
#include <rocksdb/db.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>
#include <rocksdb/utilities/transaction.h>

#include <assert.h>
#include <spdlog/spdlog.h>
#include <user_engine.pb.h>

namespace yuzhi::meta {
using namespace rocksdb;

UsersImpl::UsersImpl(std::weak_ptr<rocksdb::DB> db)
    : db_(std::move(db)) {
  using namespace rocksdb;

  if (auto db =
          std::dynamic_pointer_cast<rocksdb::OptimisticTransactionDB>(
              db_.lock());
      db) {
    rocksdb::ColumnFamilyOptions options;
    if (auto status = db->CreateColumnFamily(options, genUserColumnFamilyName(),
                                             &cf_handle_);
        !status.ok()) {
      assert(false);
      SPDLOG_ERROR("Error create name {} column family handle",
                   genUserColumnFamilyName());
    }
  }
}

UsersImpl::~UsersImpl() { assert(!db_.lock() && !cf_handle_); }

void UsersImpl::IDistory() {

  if (auto db =
          std::dynamic_pointer_cast<ROCKSDB_NAMESPACE::OptimisticTransactionDB>(
              db_.lock());
      db) {

    db->DestroyColumnFamilyHandle(cf_handle_);
    cf_handle_ = nullptr;
  }
}

// create user object
std::optional<common::Error>
UsersImpl::createUser(const std::string &user_name,
                      const std::string &ledger_name) {

  using namespace rocksdb;

  if (auto db =
          std::dynamic_pointer_cast<ROCKSDB_NAMESPACE::OptimisticTransactionDB>(
              db_.lock());
      db) {

    user_engine::User user;
    user.set_name(user_name);
    user.set_attachment_ledger(ledger_name);

    auto txn_db = db->BeginTransaction(ROCKSDB_NAMESPACE::WriteOptions{});

    std::string val;
    if (auto status = txn_db->Get(ROCKSDB_NAMESPACE::ReadOptions{}, cf_handle_,
                                  genCurrentUserIdOfKey(), &val);
        !status.ok()) {
      // txn_db->Put(ROCKSDB_NAMESPACE::WriteOptions{}, cf_handle_,
      //             genCurrentUserIdOfKey(), "0");
    }

    txn_db->Commit();
  }
  return std::nullopt;
}

// delete user object
std::optional<common::Error>
UsersImpl::deleteUser(const std::string &user_name) {
  return std::nullopt;
}

} // namespace yuzhi::meta