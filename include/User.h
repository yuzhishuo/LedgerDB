#pragma once
#include <string>
#include <map>
#include <set>
#include <memory>


enum class USER_ROLE : uint8_t
{
    DBA,
    REGULATOR,
    COMMON
};


class User
{
public:
    User(const std::string &name, USER_ROLE role = USER_ROLE::COMMON)
        : id_{User::GeneratorId()}, name_(name), role_(role)
    {}

    
    
    const std::string &name() const
    {
        return name_;
    }

    uint64_t id() const
    {
        return id_;
    }

    USER_ROLE role() const
    {
        return role_;
    }

    static uint64_t GeneratorId()
    {
        static uint64_t id = 0;
        return ++id;
    }

    bool operator<(const User &rhs) const
    {
        return id_ < rhs.id_;
    }

public:
    static std::shared_ptr<User> admin;
    static auto &Users()
    {
        static std::map<std::string, std::shared_ptr<User>> users{std::make_pair(admin->name(), admin)};
        return users;
    }

private:
    user_engine::User user_proto_imp;
    uint64_t id_;
    std::string name_;
    USER_ROLE role_;
};

