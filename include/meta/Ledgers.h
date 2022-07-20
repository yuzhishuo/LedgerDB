/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-20 07:13:01
 */
#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <type_traits>

#include "meta/Ledger.h"
#include "meta/LedgersImpl.h"
#include "interfaces/IStorable.h"
#include "store/PersistenceStore.h"
#include "ledger_engine.pb.h"
#include "LedgerStoreCreator.h"

class User;

constexpr auto kLedgerStoreName = "ledgerdb";

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
          impl_(kLedgerStoreName),
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

public: //  Engine
    
public: // IStorable
    virtual std::optional<Error> store(const Element &element) const override
    {
        if(auto err = store_creator_->store(element); err) 
        {
            return err;
        }

        return std::nullopt;
    }

    virtual Element load(const std::shared_ptr<IUnique<UniqueType>> &element) const override
    {
        return store_creator_->load(element);
    }

public:
    std::shared_ptr<Ledger> createLedger(const std::string &name, const std::string &owner);
    bool removeLedger(const std::string& ledger_name);
    bool removeLedger(const std::shared_ptr<Ledger>& ledger);
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

    yuzhi::LedgersImpl impl_;
};