/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-03-11 01:56:12
 */
#pragma once

#include <string>
#include <mutex>

#include "meta/User.h"
#include "meta/Ledger.h"
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
