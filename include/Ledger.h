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
#include "IUnique.h"
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

class Ledger final : public IDisposable, public Uint64TUnique, public std::enable_shared_from_this<Ledger>
{
public:
    Ledger(const std::string &name, const std::string &owner);
    Ledger(ledger_engine::Ledger &&ledger_inner);

    virtual ~Ledger() = default;

    virtual void dispose() override;

    // void setOwner(std::shared_ptr<User> &owner);

    // std::shared_ptr<User> owner() const;

    virtual uint64_t GetUnique() const override
    {
        return std::get<ledger_engine::Ledger>(ledger_).id();
    }

    LEDGER_ROLE GetRoleByUserName(const std::string &name) const;

    bool isOwner(const std::string &name) const;
    bool isCommon(const std::string &name) const;
    bool isRegulator(const std::string &name) const;
    bool isReadOnly(const std::string &name) const;

    std::shared_ptr<User> Onwer() const;
    // std::optional<Error> addRegulator(const std::string &name);

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

private:
    std::variant<std::monostate, ledger_engine::Ledger> ledger_;
};
