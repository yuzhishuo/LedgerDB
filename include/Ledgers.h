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

class User;

class Ledgers : public IStorable<Ledger>
{
public:
    using Raw = Ledger;
    using Element = std::shared_ptr<Ledger>;
    using UniqueType = std::common_type_t<decltype(((Ledger *)nullptr)->GetUnique())>;

public:
    static Ledgers &getInstance()
    {
        static Ledgers instance;
        return instance;
    }

public: // IStorable
    virtual std::optional<Error> store(const Element &element) const override
    {
        if (auto [raw, err] = element->serialize(); !err)
        {
            return PersistenceStore::getInstance().save("ledger_" + element->GetUnique(), raw);
            return std::nullopt;
        }
        else
        {
            return err;
        }
    }

    virtual Element load(const std::shared_ptr<IUnique<UniqueType>> &element) override
    {

        if (auto ledger = getLedger(element->GetUnique()); ledger)
        {
            return ledger;
        }
        else
        {
            auto &store_instance = PersistenceStore::getInstance();

            auto [store_raw, err] = store_instance.load("ledger_" + element->GetUnique());

            if (err)
            {
                std::cout << "Ledgers::load error" << err->message() << std::endl;
            }

            ledger_engine::Ledger ledger_inner;
            if (auto parse_result = ledger_inner.ParseFromString(store_raw); parse_result)
            {
                auto ledger = std::make_shared<Ledger>(std::move(ledger_inner));
                // store(ledger);
                return ledger;
            }
            else
            {
                std::cout << "Ledgers::load error parse fail" << std::endl;
            }
        }

        return nullptr;
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
};