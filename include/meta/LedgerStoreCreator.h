/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:16:12
 */
#pragma once

#include "meta/Ledger.h"
#include "store/PersistenceStore.h"
#include "store/StoreCreator.h"
namespace yuzhi
{
class LedgerStoreCreator : public store::StoreCreator<Ledger>
{
public:
  using store::StoreCreator<Ledger>::StoreCreator;

public:
  virtual store::IStorage *create() const override
  {
    static auto persistenceStore = store::PersistenceStore("Ledger", "db0");
    return &persistenceStore;
  }
};
} // namespace yuzhi