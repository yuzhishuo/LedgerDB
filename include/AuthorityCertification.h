#pragma once

#include <string>
#include <mutex>

#include "User.h"
#include "Ledger.h"
#include "Strategy.h"
#include "UserStrategy.h"
#include "LedgerStrategy.h"

class AuthorityCertification
{
public:
    AuthorityCertification() {}

    bool UserPass(const std::string &command_name, const std::string &user_name) const;

    bool LedgerPass(const std::string &command_name, const std::string &user_name, const std::string &ledger_name) const;

    bool RegisterStrategy(std::unique_ptr<IStrategy> &&strategy);

public:
    static void Start()
    {
        UserAuthorityCertificationHelper;
        LedgerAuthorityCertificationHelper;
    }

    static AuthorityCertification &Instance()
    {
        static AuthorityCertification instance;
        return instance;
    }

private:
    std::map<std::string, std::unique_ptr<IStrategy>> strategys_;
};
