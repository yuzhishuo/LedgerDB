/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-21 08:40:53
 */
#pragma once

#include "meta/User.h"
#include "store/PersistenceStore.h"
#include "store/StoreCreator.h"

namespace yuzhi {
class UserStoreCreator : public store::StoreCreator<User> {
public:
  using StoreCreator<User>::StoreCreator;

public:
  virtual store::IStorage *create() const override {
    static auto persistenceStore = store::PersistenceStore("User", "db1");
    return &persistenceStore;
  }
};
} // namespace yuzhi
