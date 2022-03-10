/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-03-11 01:58:52
 */
#include "meta/Users.h"

#include "meta/User.h"

std::shared_ptr<User> Users::createUser(const std::string &name)
{
    if (auto itr = users_.find(name); itr != users_.end())
    {
        return nullptr;
    }

    auto user = std::make_shared<User>(name);
    users_.insert(std::make_pair(name, user));
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