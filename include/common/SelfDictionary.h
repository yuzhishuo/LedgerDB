/*
 * @Author: Leo
 * @Date: 2022-02-01 21:47:19
 * @LastEditTime: 2022-07-22 09:48:04
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/common/SelfDictionary.h
 */
#pragma once
#include <map>
#include <memory>
#include <optional>
#include <utility>

#include "Error.h"
#include "interfaces/IUnique.h"

namespace yuzhi
{

template <typename T, typename U = typename T::Key> class SelfDictionary
{

public:
  using Key = U;
  using Value = T;

  static_assert(std::is_base_of_v<IUnique<Key>, T>);
  static_assert(std::is_default_constructible_v<Key>);
  static_assert(std::is_copy_constructible_v<Key>);
  static_assert(std::is_copy_assignable_v<Key>);

public:
  SelfDictionary() = default;
  ~SelfDictionary() = default;

public:
  std::optional<common::Error> Add(const Value &value)
  {

    auto is_inserted = dictionary_.insert(std::make_pair(value.GetUnique(), value));
    if (!is_inserted.second)
    {
      return common::Error("Duplicate key");
    }
    return std::nullopt;
  }

  std::optional<common::Error> Update(const Value &value)
  {
    if (dictionary_.find(value.GetUnique()) == dictionary_.end())
    {
      return common::Error("Key not found");
    }

    dictionary_.insert(std::move(std::make_pair(value.GetUnique(), value)));

    return std::nullopt;
  }

  std::optional<common::Error> Remove(const Value &value)
  {
    if (dictionary_.erase(value.GetUnique()) == 0)
    {
      return common::Error("Key not found");
    }
    return std::nullopt;
  }

  std::optional<common::Error> Remove(const Key &key)
  {

    if (dictionary_.erase(key) == 0)
    {
      return common::Error("Key not found");
    }
    return std::nullopt;
  }

private:
  std::map<Key, Value> dictionary_;
};

template <typename T> class SelfDictionary<std::shared_ptr<T>, typename T::Key>
{

public:
  using Key = typename T::Key;
  using Value = T;
  using SmartValue = std::shared_ptr<T>;

  static_assert(std::is_base_of_v<IUnique<Key>, T>);
  static_assert(std::is_default_constructible_v<Key>);
  static_assert(std::is_copy_constructible_v<Key>);
  static_assert(std::is_copy_assignable_v<Key>);

public:
  SelfDictionary() = default;
  ~SelfDictionary() = default;

public:
  std::optional<common::Error> Add(SmartValue &value)
  {

    auto is_inserted = dictionary_.insert(std::make_pair(value->GetUnique(), value));
    if (!is_inserted.second)
    {
      return common::Error::RepeatKey();
    }
    return std::nullopt;
  }

  std::optional<common::Error> Update(SmartValue &value)
  {
    if (dictionary_.find(value->GetUnique()) == dictionary_.end())
    {
      return common::Error("Key not found");
    }

    dictionary_.insert(std::move(std::make_pair(value->GetUnique(), value)));

    return std::nullopt;
  }

  std::optional<common::Error> Remove(SmartValue &value)
  {
    if (dictionary_.erase(value->GetUnique()) == 0)
    {
      return common::Error("Key not found");
    }
    return std::nullopt;
  }

  std::optional<common::Error> Remove(const Key &key)
  {

    if (dictionary_.erase(key) == 0)
    {
      return common::Error("Key not found");
    }
    return std::nullopt;
  }

  SmartValue Get(const Key &key)
  {
    if (dictionary_.count(key) > 0)
    {
      return dictionary_.at(key);
    }
    return nullptr;
  }

private:
  std::map<Key, SmartValue> dictionary_;
};

// using  StringUniqueSelfDictionary =  SelfDictionary<IUnique<std::string>>;
// using IntUniqueSelfDictionary = SelfDictionary<IUnique<int>>;
// using Uint64TUniqueSelfDictionary = SelfDictionary<IUnique<uint64_t>>;
// using Uint32TUniqueSelfDictionary = SelfDictionary<IUnique<uint32_t>>;
// using Uint16TUniqueSelfDictionary = SelfDictionary<IUnique<uint16_t>>;
// using Uint8TUniqueSelfDictionary = SelfDictionary<IUnique<uint8_t>>;
// using CharUniqueSelfDictionary = SelfDictionary<IUnique<char>>;
// using BoolUniqueSelfDictionary = SelfDictionary<IUnique<bool>>;
// using FloatUniqueSelfDictionary = SelfDictionary<IUnique<float>>;
// using DoubleUniqueSelfDictionary = SelfDictionary<IUnique<double>>;

} // namespace yuzhi
