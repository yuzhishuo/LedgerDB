/*
 * @Author: Leo
 * @Date: 2022-07-21 07:36:48
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:49:35
 */
#include "meta/UsersImpl.h"
#include "meta/Constant.h"
#include <rocksdb/db.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>
#include <rocksdb/utilities/transaction.h>

#include <assert.h>
#include <spdlog/spdlog.h>
#include <user_engine.pb.h>

namespace yuzhi::meta
{
using namespace rocksdb;

UsersImpl::UsersImpl(std::weak_ptr<rocksdb::DB> db) : db_(std::move(db))
{
  using namespace rocksdb;

  if (auto db = std::dynamic_pointer_cast<rocksdb::OptimisticTransactionDB>(db_.lock()); db)
  {

    SPDLOG_INFO("Create column family {}", genUserColumnFamilyName());
    rocksdb::ColumnFamilyOptions options;
    if (auto status = db->CreateColumnFamily(options, genUserColumnFamilyName(), &cf_handle_); !status.ok())
    {
      assert(CreateThreadStatusUpdater);
      SPDLOG_ERROR("Error create name {} column family handle", genUserColumnFamilyName());
      goto end;
    }

    std::string val;
    if (auto status = db->Get(ReadOptions(), cf_handle_, genCurrentUserIdOfKey(), &val); status == status.NotFound())
    {
      SPDLOG_INFO("Create current user id key {}", genCurrentUserIdOfKey());
      if (auto status = db->Put(WriteOptions(), genCurrentUserIdOfKey(), "0"); !status.ok())
      {
        assert(true);
        SPDLOG_ERROR("Error create current user id key {}", genCurrentUserIdOfKey());
        goto end;
      }
    }
  }
end:
  SPDLOG_ERROR("Error creating current user id key {}", genCurrentUserIdOfKey());
}

UsersImpl::~UsersImpl() { assert(!db_.lock() && !cf_handle_); }

void UsersImpl::dispose()
{

  if (auto db = std::dynamic_pointer_cast<ROCKSDB_NAMESPACE::OptimisticTransactionDB>(db_.lock()); db)
  {

    db->DestroyColumnFamilyHandle(cf_handle_);
    cf_handle_ = nullptr;
    return;
  }

  assert(true);
}

// create user object
std::optional<common::Error> UsersImpl::createUser(const std::string &user_name, const std::string &ledger_name) const
{

  using namespace rocksdb;

  if (auto db = std::dynamic_pointer_cast<ROCKSDB_NAMESPACE::OptimisticTransactionDB>(db_.lock()); db)
  {

    user_engine::User user;
    user.set_name(user_name);
    user.set_attachment_ledger(ledger_name);

    auto txn_db = db->BeginTransaction(ROCKSDB_NAMESPACE::WriteOptions{});

    std::string val;
    if (auto status = txn_db->GetForUpdate(ROCKSDB_NAMESPACE::ReadOptions{}, cf_handle_, genCurrentUserIdOfKey(), &val);
        status.ok())
    {

      auto current_user_id = std::stoi(val);

      user.set_id(current_user_id);

      if (auto status = txn_db->Put(cf_handle_, genUserStoreOfKey(ledger_name, user_name), user.SerializeAsString());
          !status.ok())
      {
        assert(false);
        SPDLOG_ERROR("Error create user {}", user_name);
        return common::Error::InternalError("Error create user " + user_name);
      }

      if (auto status = txn_db->Put(cf_handle_, genCurrentUserIdOfKey(), std::to_string(current_user_id + 1));
          !status.ok())
      {
        assert(false);
        SPDLOG_ERROR("Error create current user id key {}", genCurrentUserIdOfKey());
        return common::Error::InternalError("Error create current user id key " + genCurrentUserIdOfKey());
      }
    }

    txn_db->Commit();
  }
  return std::nullopt;
}

// delete user object
std::optional<common::Error> UsersImpl::deleteUser(const std::string &ledger_name, const std::string &user_name) const
{

  using namespace rocksdb;
  if (auto db = std::dynamic_pointer_cast<ROCKSDB_NAMESPACE::OptimisticTransactionDB>(db_.lock()); db)
  {

    auto txn_db = db->BeginTransaction(ROCKSDB_NAMESPACE::WriteOptions{});
    std::string val;
    txn_db->GetForUpdate(ROCKSDB_NAMESPACE::ReadOptions{}, cf_handle_, genUserStoreOfKey(ledger_name, user_name), &val);
    auto user = user_engine::User{};
    // TODO: check user exists and set flag of delete
    user.ParseFromString(val);
    user.SerializeToString(&val);
    txn_db->Put(cf_handle_, genUserStoreOfKey(ledger_name, user_name), val);
    txn_db->Commit();
  }
  return std::nullopt;
}
} // namespace yuzhi::meta