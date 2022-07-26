/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-23 16:42:19
 */
#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <any>
#include <utility>

class IStrategy
{

public:
  IStrategy(const std::string &name, std::vector<std::any> &&r) : name_(name), roles_(std::move(r)) {}
  virtual ~IStrategy() {}

  IStrategy &operator=(const IStrategy &) = delete;

public:
  bool Pass(std::any role) const
  {
    for (const auto &r : roles_)
    {
      if (!role.has_value())
      {
        return false;
      }
      if (r.type() == role.type() && eq(r, role))
      {
        return true;
      }
    }

    return false;
  }

  virtual bool eq(const std::any &le, const std::any &ri) const = 0;

  const std::string &name() const { return name_; }

private:
  std::vector<std::any> roles_;
  const std::string name_;
};
