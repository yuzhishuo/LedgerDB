/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-17 01:36:26
 */
#include "rbac/AuthorityCertification.h"
#include "meta/User.h"
#include "meta/Users.h"
#include "meta/Ledgers.h"

bool AuthorityCertification::UserPass(const std::string &command_name, const std::string &user_name) const
{
    if (auto command = strategys_.find(command_name); command != strategys_.end())
    {
        const auto &users = Users::getInstance();
        if (const auto &user = users.getUser(user_name); user != nullptr)
        {
            return command->second->Pass(user->role());
        }
    }
    return false;
}

bool AuthorityCertification::LedgerPass(const std::string &command_name, const std::string &user_name, const std::string &ledger_name) const
{

    if (auto command = strategys_.find(command_name); command != strategys_.end())
    {
        auto &ledgers = Ledgers::getInstance();

        if (const auto &ledger = ledgers.getLedger(ledger_name); ledger != nullptr)
        {
            return command->second->Pass(ledger->getRoleByUserName(user_name));
        }
    }
    return false;
}

bool AuthorityCertification::RegisterStrategy(std::unique_ptr<IStrategy> &&strategy)
{
    if (strategys_.find(strategy->name()) != strategys_.end())
    {
        return false;
    }

    auto name = strategy->name();
    strategys_.insert(std::make_pair(name, std::move(strategy)));
    return true;
}