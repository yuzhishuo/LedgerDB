#pragma once

#include <vector>
#include <memory>

#include "Ledger.h"

class Ledgers
{

public:
    static Ledgers &getInstance()
    {
        static Ledgers instance;
        return instance;
    }

public:
    std::shared_ptr<Ledger> createLedger(const std::string &name, const std::string &owner);
    bool removeLedger(const std::shared_ptr<Ledger> &ledger);
    bool hasLedger(const std::string &name) const;

    std::shared_ptr<Ledger> getLedger(const std::string &name) const;

private:
    std::map<std::string, std::shared_ptr<Ledger>> ledgers_;
};