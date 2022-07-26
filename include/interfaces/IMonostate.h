/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:48:27
 */
#pragma once

#include <optional>
#include <type_traits>

#include "IUnique.h"
#include "common/Error.h"

namespace yuzhi
{
template <typename T> class IMonostate
{
public:
  virtual ~IMonostate() = default;
  virtual std::pair<std::string, std::optional<common::Error>> serialize() const = 0;
  virtual std::pair<std::shared_ptr<T>, std::optional<common::Error>> deserialize(const std::string &serialized) = 0;
};

// duck model
template <typename T> class Monostate : public IMonostate<T>
{
public:
  Monostate() = default;
  ~Monostate() = default;
  Monostate(const T &t) : value_(t) {}
  Monostate(T &&t) : value_(std::move(t)) {}

private:
  Monostate(const Monostate &other) = delete;
  Monostate(Monostate &&other) = delete;
  Monostate &operator=(const Monostate &other) = delete;

public:
  virtual std::pair<std::string, std::optional<common::Error>> serialize() const override
  {
    if (auto serialized = std::string(); value_.SerializeToString(&serialized))
    {
      return {serialized, std::nullopt};
    }

    return std::make_pair(std::string{}, common::Error{"Failed to serialize Monostate"});
  }

  virtual std::pair<std::shared_ptr<T>, std::optional<common::Error>>
  deserialize(const std::string &serialized) override
  {
    if (value_.ParseFromString(serialized))
    {
      return {std::make_shared<T>(value_), std::nullopt};
    }
    else
    {
      return {nullptr, common::Error{"Failed to deserialize Monostate"}};
    }
  }

public:
  T *operator->() { return std::addressof(value_); }
  const T *operator->() const { return std::addressof(value_); }
  T &operator*() { return value_; }
  const T &operator*() const { return value_; }

private:
  T value_;
};
} // namespace yuzhi