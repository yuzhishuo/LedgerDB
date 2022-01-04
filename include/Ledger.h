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
#include "interfaces/IMonostate.h"
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

class Ledger final : public IDisposable, public StringUnique, public std::enable_shared_from_this<Ledger>, public IMonostate<ledger_engine::Ledger>
{

public:
    Ledger(const std::string &name, const std::string &owner);
    Ledger(ledger_engine::Ledger &&ledger_inner);

    virtual ~Ledger() = default;

    virtual void dispose() override;

    // void setOwner(std::shared_ptr<User> &owner);

    // std::shared_ptr<User> owner() const;

    virtual const std::string &GetUnique() const override
    {
        return ledger_->name();
    }

    LEDGER_ROLE GetRoleByUserName(const std::string &name) const;

    bool isOwner(const std::string &name) const;
    bool isCommon(const std::string &name) const;
    bool isRegulator(const std::string &name) const;
    bool isReadOnly(const std::string &name) const;

    std::shared_ptr<User> Onwer() const;

    // std::optional<Error> addRegulator(const std::string &name);
    // std::optional<Error> addCommon(const std::string &name);
    // std::optional<Error> addReadOnly(const std::string &name);

    std::optional<Error> removeCommon(const std::string &name);
    std::optional<Error> removeRegulator(const std::string &name);
    std::optional<Error> removeReadOnly(const std::string &name);

    const std::string &name() const;
    uint64_t id() const;

public:
    virtual std::pair<const std::string &, std::optional<Error>> serialize() const override
    {
        return ledger_.serialize();
    }
    
    virtual std::pair<std::shared_ptr<ledger_engine::Ledger>, std::optional<Error>> deserialize(const std::string &serialized) override
    {
        return ledger_.deserialize(serialized);
    }

public:
    static uint64_t GeneratorId()
    {
        static uint64_t id = 0;
        return ++id;
    }

private:
    Monostate<ledger_engine::Ledger> ledger_;
};
