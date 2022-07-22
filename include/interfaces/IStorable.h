/*
 * @Author: Leo
 * @Date: 2022-07-17 00:23:49
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:15:06
 */
#pragma once

#include <type_traits>

#include "IUnique.h"
#include <common/Error.h>
namespace yuzhi::store
{
template <typename T> class IStorable
{

public:
  static_assert(std::is_base_of_v<IUnique<typename T::Key>, T>,
                "T must be a subclass of IUnique");

public:
  virtual ~IStorable() {}
  virtual std::optional<common::Error>
  store(const std::shared_ptr<T> &object) const = 0;
  virtual std::shared_ptr<T>
  load(const std::shared_ptr<IUnique<typename T::Key>> &object) const = 0;
};
} // namespace yuzhi::store