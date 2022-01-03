#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Ledger.h"

class User;

class Ledgers
{
public:
    using Raw = Ledger;
    using Element = std::shared_ptr<Ledger>;
    using Container = std::vector<Element>;
    using UniqueType = decltype(((Ledger *)nullptr)->GetUnique());

public:
    static Ledgers &getInstance()
    {
        static Ledgers instance;
        return instance;
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