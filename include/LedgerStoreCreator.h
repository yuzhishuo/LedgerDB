/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-03-11 01:52:06
 */
#pragma once

#include "meta/Ledger.h"
#include "store/StoreCreator.h"

class LedgerStoreCreator : public StoreCreator<Ledger>
{
public:
    using StoreCreator<Ledger>::StoreCreator;

public:
    virtual IStorage *create() const override
    {
        static auto persistenceStore = PersistenceStore("Ledger", "db0");
        return &persistenceStore;
    }
};
