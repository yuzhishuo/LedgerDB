/*
 * @Author: Leo
 * @Date: 2022-07-23 13:17:21
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-23 13:25:44
 */

#pragma once

#include <common/Error.h>
#include <memory>
#include <optional>
#include <string>

namespace yuzhi
{
class User;
}

namespace yuzhi::meta
{

class IAccountAtrribute
{

public:
  virtual bool isOwner(const std::string &ledger_name, const std::string &name) const = 0;
  virtual bool isCommon(const std::string &ledger_name, const std::string &name) const = 0;
  virtual bool isRegulator(const std::string &ledger_name, const std::string &name) const = 0;
  virtual bool isReadOnly(const std::string &ledger_name, const std::string &name) const = 0;

  virtual std::shared_ptr<User> Onwer(const std::string &ledger_name) const = 0;

  virtual std::optional<common::Error> addCommon(const std::string &ledger_name, const std::string &name) = 0;
  virtual std::optional<common::Error> addRegulator(const std::string &ledger_name, const std::string &name) = 0;
  virtual std::optional<common::Error> addReadOnly(const std::string &ledger_name, const std::string &name) = 0;

  virtual std::optional<common::Error> removeCommon(const std::string &ledger_name, const std::string &name) = 0;
  virtual std::optional<common::Error> removeRegulator(const std::string &ledger_name, const std::string &name) = 0;
  virtual std::optional<common::Error> removeReadOnly(const std::string &ledger_name, const std::string &name) = 0;
};
} // namespace yuzhi::meta
