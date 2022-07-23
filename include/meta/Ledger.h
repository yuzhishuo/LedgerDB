/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-23 14:31:12
 */
#pragma once

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <variant>

#include <ledger_engine.pb.h>

#include "common/Error.h"
#include "interfaces/IDisposable.h"
#include "interfaces/IMonostate.h"
#include "interfaces/IUnique.h"
#include "meta/User.h"

enum class LEDGER_ROLE : uint8_t
{
  UNKION,
  OWNER,
  REGULATOR,
  COMMON,
  READONLY
};

namespace yuzhi
{

namespace meta 
{
  class IAccountAtrribute;
}
class LedgerEngine;

class Ledger final : public interface::IDisposable,
                     public StringUnique,
                     public std::enable_shared_from_this<Ledger>,
                     public IMonostate<ledger_engine::Ledger>
{
public:
  using MonoType = ledger_engine::Ledger;

public: // meta
  Ledger(meta::IAccountAtrribute* attribute, const std::string &name, const std::string &owner);
  Ledger(meta::IAccountAtrribute* attribute, ledger_engine::Ledger &&ledger_inner);

  virtual ~Ledger() = default;

  virtual void dispose() override;

  // void setOwner(std::shared_ptr<User> &owner);

  // std::shared_ptr<User> owner() const;

  virtual const std::string &GetUnique() const override { return ledger_->name(); }

  LEDGER_ROLE getRoleByUserName(const std::string &name) const;

  bool isOwner(const std::string &name) const;
  bool isCommon(const std::string &name) const;
  bool isRegulator(const std::string &name) const;
  bool isReadOnly(const std::string &name) const;

  std::shared_ptr<User> Onwer() const;

  std::optional<common::Error> addCommon(const std::string &name);
  std::optional<common::Error> addRegulator(const std::string &name);
  std::optional<common::Error> addReadOnly(const std::string &name);

  std::optional<common::Error> removeCommon(const std::string &name);
  std::optional<common::Error> removeRegulator(const std::string &name);
  std::optional<common::Error> removeReadOnly(const std::string &name);

  const std::string &name() const;
  uint64_t id() const;

public: // engine
  std::shared_ptr<yuzhi::LedgerEngine> engine();

public:
  std::pair<std::string, std::optional<common::Error>> serialize() const override
  {
    return ledger_.serialize();
  }

  std::pair<std::shared_ptr<ledger_engine::Ledger>, std::optional<common::Error>>
  deserialize(const std::string &serialized) override
  {
    return ledger_.deserialize(serialized);
  }

public:
  static uint64_t generatorId()
  {
    static uint64_t id = 0;
    return ++id;
  }

private:
  Monostate<MonoType> ledger_;
  meta::IAccountAtrribute* attribute_;
};
} // namespace yuzhi