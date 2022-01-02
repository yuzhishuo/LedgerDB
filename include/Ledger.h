#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>
#include <variant>
#include <optional>
#include <string>

#include <ledger_engine.pb.h>

#include "Error.h"
#include "IDisposable.h"

enum class LEDGER_ROLE : uint8_t
{
    UNKION,
    OWNER,
    REGULATOR,
    COMMON,
    READONLY
};

class User;

class Ledger final : public IDisposable, public std::enable_shared_from_this<Ledger>
{
public:
    Ledger(const std::string &name, const std::string &owner);
    virtual ~Ledger() = default;
    
    virtual void dispose() override;

    // void setOwner(std::shared_ptr<User> &owner);

    // std::shared_ptr<User> owner() const;

    LEDGER_ROLE GetRoleByUserName(const std::string &name) const;

    bool isOwner(const std::string &name) const;
    bool isCommon(const std::string &name) const;
    bool isRegulator(const std::string &name) const;
    bool isReadOnly(const std::string &name) const;

    std::optional<Error> removeCommon(const std::string &name);
    std::optional<Error> removeRegulator(const std::string &name);
    std::optional<Error> removeReadOnly(const std::string &name);

    const std::string &name() const;
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
    std::variant<std::monostate, ledger_engine::Ledger> ledger_;
    std::string owner_;
};
