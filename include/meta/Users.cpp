/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-18 10:05:58
 */
#include "Users.h"
#include "User.h"

std::shared_ptr<User> Users::createUser(const std::string &user_name, const std::string &ledger_name, USER_ROLE role)
{
    if (auto itr = users_.find(user_name); itr != users_.end())
    {
        return nullptr;
    }

    auto user = std::make_shared<User>(user_name, ledger_name, role);
    users_.insert(std::make_pair(user_name, user));
    return user;
}

bool Users::removeUser(const std::shared_ptr<User> &user)
{
    auto itr = users_.find(user->name());
    if (itr == users_.end())
    {
        return false;
    }

    users_.erase(itr);
    // assert(itr->second.unique());
    return true;
}
    

const std::shared_ptr<User> Users::getUser(const std::string &name) const
{
    if (auto itr = users_.find(name); itr != users_.end())
    {
        return itr->second;
    }
    return nullptr;
}

std::shared_ptr<User> Users::getUser(const std::string &name)
{
    if (auto itr = users_.find(name); itr != users_.end())
    {
        return itr->second;
    }
    return nullptr;
}