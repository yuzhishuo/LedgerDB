/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-17 09:23:10
 */
#pragma once

#include <map>
#include <memory>
#include <initializer_list>

#include <spdlog/spdlog.h>

#include "meta/User.h"
#include "UserStoreCreator.h"

// #define PREDEF_USER "admin"

class Users : public IStorable<User>
{
public:
    using Raw = User;
    using Element = std::shared_ptr<User>;
    using UniqueType = std::common_type_t<decltype(((User *)nullptr)->GetUnique())>;

public:
    static Users &getInstance()
    {
        static Users instance{
            std::make_pair("admin", std::make_shared<User>("admin", USER_ROLE::DBA))};
        return instance;
    }

private:
    Users(std::initializer_list<std::pair<std::string, std::shared_ptr<User>>> init)
        : users_{},  store_creator_{dynamic_cast<IStorable<User> *>(new UserStoreCreator{"User"})}
    {
        for (auto &[name, user] : init)
        {
            if(auto erro = store(user); !erro)
            {
                users_.insert(std::make_pair(name, user));
            } 
            else
            {
                
            }
        }
    }

public: // IStorable
    virtual std::optional<Error> store(const Element &element) const override
    {
        return store_creator_->store(element);
    }

    virtual Element load(const std::shared_ptr<IUnique<UniqueType>> &element) const override
    {
        return store_creator_->load(element);
    }

public:
    std::shared_ptr<User> createUser(const std::string &name);
    std::shared_ptr<User> getUser(const std::string &name);
    const std::shared_ptr<User> getUser(const std::string &name) const;
    bool removeUser(const std::shared_ptr<User> &user);

private:
    std::map<std::string, std::shared_ptr<User>> users_;
    std::unique_ptr<IStorable<User>> store_creator_;
};