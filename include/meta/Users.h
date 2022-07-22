/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 06:07:02
 */
#pragma once

#include <initializer_list>
#include <interfaces/IDisposable.h>
#include <map>
#include <memory>
#include <rocksdb/db.h>
#include <spdlog/spdlog.h>

#include "UserStoreCreator.h"
#include "meta/User.h"
#include "meta/UsersImpl.h"

// #define PREDEF_USER "admin"
namespace yuzhi {
class Users : public store::IStorable<User>, interface::IDisposable {
public:
  using Raw = User;
  using Element = std::shared_ptr<User>;
  using UniqueType =
      std::common_type_t<decltype(((User *)nullptr)->GetUnique())>;

public:
  static Users &getInstance() {
    static Users instance{};
    return instance;
  }

  void dispose() override { usersImpl_.dispose(); }

public:
  Users(
      std::initializer_list<std::pair<std::string, std::shared_ptr<User>>> init)
      : users_{}, store_creator_{dynamic_cast<IStorable<User> *>(
                      new UserStoreCreator{"User"})},
        usersImpl_(std::weak_ptr<ROCKSDB_NAMESPACE::DB>()) {
    for (auto &[name, user] : init) {
      if (auto erro = store(user); !erro) {
        users_.insert(std::make_pair(name, user));
      } else {
      }
    }
  }

  Users(std::weak_ptr<ROCKSDB_NAMESPACE::DB> db)
      : users_{}, store_creator_{dynamic_cast<IStorable<User> *>(
                      new UserStoreCreator{"User"})},
        usersImpl_(db) {}

public: // IStorable
  virtual std::optional<common::Error>
  store(const Element &element) const override {
    return store_creator_->store(element);
  }

  virtual Element
  load(const std::shared_ptr<IUnique<UniqueType>> &element) const override {
    return store_creator_->load(element);
  }

public:
  std::shared_ptr<User> createUser(const std::string &user_name,
                                   const std::string &ledger_name,
                                   USER_ROLE role);
  std::shared_ptr<User> getUser(const std::string &name);
  const std::shared_ptr<User> getUser(const std::string &name) const;
  bool removeUser(const std::shared_ptr<User> &user);

private:
  std::map<std::string, std::shared_ptr<User>> users_;
  std::unique_ptr<IStorable<User>> store_creator_;
  yuzhi::meta::UsersImpl usersImpl_;
};
} // namespace yuzhi