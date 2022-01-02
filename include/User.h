#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>
#include <variant>

#include <user_engine.pb.h>

#include "IDisposable.h"

enum class USER_ROLE : uint8_t
{
    DBA,
    REGULATOR,
    COMMON
};

class User final : public std::enable_shared_from_this<User>, public IDisposable
{
public:
    User(const std::string &name, USER_ROLE role = USER_ROLE::COMMON)
        : id_{User::GeneratorId()}, name_(name), role_(role) {}
    virtual ~User() = default;
    
    virtual void dispose() override;

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
    void update_password(const std::string &password)
    {
        std::get<user_engine::User>(user_).set_password(password);
    }

    void update_public_key(const std::string &public_key)
    {
        std::get<user_engine::User>(user_).set_public_key(public_key);
    }

private:
    uint64_t id_;
    std::variant<uint64_t, user_engine::User> user_;
    std::string name_;
    USER_ROLE role_;
};
