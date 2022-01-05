#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <type_traits>

#include "Ledger.h"
#include "interfaces/IStorable.h"
#include "PersistenceStore.h"
#include "ledger_engine.pb.h"
#include "LedgerStoreCreator.h"

class User;

class Ledgers : public IStorable<Ledger>
{
public:
    using Raw = Ledger;
    using Element = std::shared_ptr<Ledger>;
    using UniqueType = std::common_type_t<decltype(((Ledger *)nullptr)->GetUnique())>;

public:
    Ledgers()
        : ledgers_(),
          store_creator_{dynamic_cast<IStorable<Ledger> *>(new LedgerStoreCreator{"Ledger"})},
          IStorable<Ledger>()
    {
    }

    virtual ~Ledgers() = default;

public:
    static Ledgers &getInstance()
    {
        static Ledgers instance;
        return instance;
    }

public: // IStorable
    virtual std::optional<Error> store(const Element &element) const override
    {
        return store_creator_->store(element);
    }

    virtual Element load(const std::shared_ptr<IUnique<UniqueType>> &element) override
    {
        return store_creator_->load(element);
    }

public:
    std::shared_ptr<Ledger> createLedger(const std::string &name, const std::string &owner);
    bool removeLedger(const std::shared_ptr<Ledger> &ledger);
    /**
     * @brief remove all ledgers which owner is user
     *
     * @param user ledger owner
     * @return true ledger is removed
     * @return false  no ledger is removed
     */
    bool removeLedgerByUser(const std::shared_ptr<User> &user);
    bool hasLedger(const std::string &name) const;

    std::shared_ptr<Ledger> getLedger(const std::string &name) const;

private:
    std::map<std::string, Element> ledgers_;
    std::unique_ptr<IStorable<Ledger>> store_creator_;
};