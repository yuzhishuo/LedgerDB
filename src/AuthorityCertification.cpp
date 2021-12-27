#include "AuthorityCertification.h"
#include "User.h"

bool AuthorityCertification::UserPass(const std::string &command_name, const std::string &user_name) const
{
    auto command = strategys_.find(command_name);
    if (command == strategys_.end())
    {
        return false;
    }
    const auto &users = User::Users();
    auto user = users.find(user_name);
    if (user == users.end())
    {
        return false;
    }
    return command->second->Pass(user->second->role());
}

bool AuthorityCertification::LedgerPass(const std::string &command_name, const std::string &user_name, const std::string &ledger_name) const
{

    auto command = strategys_.find(command_name);
    if (command == strategys_.end())
    {
        return false;
    }
    const auto &ledger = Ledger::Ledgers().find(ledger_name);
    if (ledger == Ledger::Ledgers().end())
    {
        return false;
    }

    return command->second->Pass(ledger->second->GetRoleByUserName(user_name));
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