/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-03-11 01:56:20
 */
#pragma once

#include "meta/User.h"
#include "store/StoreCreator.h"
#include "store/PersistenceStore.h"

class UserStoreCreator : public StoreCreator<User>
{
public:
    using StoreCreator<User>::StoreCreator;

public:
    virtual IStorage *create() const override
    {
        static auto persistenceStore = PersistenceStore("User", "db1");
        return &persistenceStore;
    }
};
