#include "AuthorityCertification.h"
#include "User.h"
#include "Users.h"
#include "Ledgers.h"

bool AuthorityCertification::UserPass(const std::string &command_name, const std::string &user_name) const
{
    auto command = strategys_.find(command_name);
    if (command == strategys_.end())
    {
        return false;
    }

    const auto &users = Users::getInstance();
    if (const auto user = users.getUser(user_name); user != nullptr)
    {
        return command->second->Pass(user->role());
    }
    return false;
}

bool AuthorityCertification::LedgerPass(const std::string &command_name, const std::string &user_name, const std::string &ledger_name) const
{

    auto command = strategys_.find(command_name);
    if (command == strategys_.end())
    {
        return false;
    }

    auto &ledgers = Ledgers::getInstance();

    if (const auto &ledger = ledgers.getLedger(ledger_name); ledger != nullptr)
    {
        return command->second->Pass(ledger->GetRoleByUserName(user_name));
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