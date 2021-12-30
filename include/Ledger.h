#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>


enum class LEDGER_ROLE : uint8_t
{
    UNKION,
    OWNER,
    REGULATOR,
    COMMON,
    READONLY
};

class User;

class Ledger
{
public:
    Ledger(const std::string &name, std::shared_ptr<User> &owner);

    void setOwner(std::shared_ptr<User> &owner);

    std::shared_ptr<User> owner() const;

    LEDGER_ROLE GetRoleByUserName(const std::string &name) const;

    bool isOwner(const std::string &name) const;
    bool isCommon(const std::string &name) const;
    bool isRegulator(const std::string &name) const;
    bool isReadOnly(const std::string &name) const;
    std::string name() const;

    uint64_t id() const;

public:
    static uint64_t GeneratorId()
    {
        static uint64_t id = 0;
        return ++id;
    }
    static auto &Ledgers()
    {
        static std::map<std::string, std::shared_ptr<Ledger>> ledgers;
        return ledgers;
    }

private:
    std::string name_;
    uint64_t id_;

    std::weak_ptr<User> owner_;
};

