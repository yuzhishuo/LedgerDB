#include "Users.h"

#include "User.h"

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