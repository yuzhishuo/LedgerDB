/*
 * @Author: Leo
 * @Date: 2022-07-17 00:23:49
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:16:37
 */
#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <variant>

#include <user_engine.pb.h>

#include "interfaces/IDisposable.h"
#include "interfaces/IMonostate.h"

using USER_ROLE = user_engine::Role;
namespace yuzhi
{
class User final : public std::enable_shared_from_this<User>,
                   public interface::IDisposable,
                   public StringUnique,
                   public IMonostate<user_engine::User>
{
public:
  using MonoType = user_engine::User;

public:
  User(const std::string &name, const std::string &ledger_name,
       USER_ROLE role = USER_ROLE::COMMON)
  {
    user_->set_name(name);
    user_->set_attachment_ledger(ledger_name);
    user_->set_role(role);
  }

  User(user_engine::User &&user_inner) : user_(std::move(user_inner)) {}

  virtual ~User() = default;

public:
  virtual const std::string &GetUnique() const { return user_->name(); }

  virtual void dispose() override;

  const std::string &name() const { return user_->name(); }

  const std::string &id() const { return name(); }

  USER_ROLE role() const { return user_->role(); }

  static uint64_t generatorId()
  {
    static uint64_t id = 0;
    return ++id;
  }

  bool operator<(const User &rhs) const
  {
    return user_->id() < rhs.user_->id();
  }

  std::string getAttachLedgerName() const { return user_->attachment_ledger(); }

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
  virtual std::pair<std::string, std::optional<common::Error>>
  serialize() const override
  {
    return user_.serialize();
  }

  virtual std::pair<std::shared_ptr<MonoType>, std::optional<common::Error>>
  deserialize(const std::string &serialized) override
  {
    return user_.deserialize(serialized);
  }

private:
  Monostate<MonoType> user_;
};

} // namespace yuzhi