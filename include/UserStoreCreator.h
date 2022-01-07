#pragma once

#include "User.h"
#include "StoreCreator.h"
#include "PersistenceStore.h"

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
