#pragma once

#include "Ledger.h"
#include "StoreCreator.h"

class LedgerStoreCreator : public StoreCreator<Ledger>
{
public:
    using StoreCreator<Ledger>::StoreCreator;

public:
    virtual IStorage *create() const override
    {
        return new PersistenceStore("Ledger");
    }
};


