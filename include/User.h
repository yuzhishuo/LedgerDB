#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>
#include <variant>

#include <user_engine.pb.h>

#include "interfaces/IMonostate.h"
#include "IDisposable.h"

enum class USER_ROLE : uint8_t
{
    DBA,
    REGULATOR,
    COMMON
};

class User final : public std::enable_shared_from_this<User>, public IDisposable, public StringUnique, public IMonostate<user_engine::User>
{
public:
    using MonoType = user_engine::User;

public:
    User(const std::string &name, USER_ROLE role = USER_ROLE::COMMON)
        : role_(role)
    {
        user_->set_name(name);
    }

    User(user_engine::User &&user_inner)
        : user_(std::move(user_inner))
    {
    }

    virtual ~User() = default;

    virtual const std::string &GetUnique() const
    {
        return user_->name();
    }

    virtual void dispose() override;

    const std::string &name() const
    {
        return user_->name();
    }

    const std::string &id() const
    {
        return name();
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
        return user_->id() < rhs.user_->id();
    }

public:
    void update_password(const std::string &password)
    {
        user_->set_password(password);
    }

    void update_public_key(const std::string &public_key)
    {
        user_->set_public_key(public_key);
    }

public:
    virtual std::pair<std::string, std::optional<Error>> serialize() const override
    {
        return user_.serialize();
    }

    virtual std::pair<std::shared_ptr<MonoType>, std::optional<Error>> deserialize(const std::string &serialized) override
    {
        return user_.deserialize(serialized);
    }

private:
    USER_ROLE role_;
    Monostate<MonoType> user_;
};
