#pragma once

#include <map>
#include <memory>
#include <initializer_list>

#include "User.h"
// #define PREDEF_USER "admin"

class Users
{
public:
    static Users &getInstance()
    {
        static Users instance{
            std::make_pair("admin", std::make_shared<User>("admin", USER_ROLE::DBA))};
        return instance;
    }

private:
    Users(std::initializer_list<std::pair<std::string, std::shared_ptr<User>>> init)
        : users_{}
    {
        for (auto &[name, user] : init)
        {
            users_.insert(std::make_pair(name, user));
        }
    }

public:
    std::shared_ptr<User> createUser(const std::string &name);
    std::shared_ptr<User> getUser(const std::string &name);
    const std::shared_ptr<User> getUser(const std::string &name) const;

private:
    std::map<std::string, std::shared_ptr<User>> users_;
};