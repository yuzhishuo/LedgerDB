/*
 * @Author: Leo
 * @Date: 2022-01-25 16:36:55
 * @LastEditTime: 2022-08-12 12:52:27
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/IUnique.h
 */
#pragma once

#include <string>
#include <utility>

#include "utility/opeartors.h"

template <typename T, typename = std::void_t<>> struct is_comparable : std::false_type
{
};

template <typename T>
struct is_comparable<T, std::void_t<
                            // TODO: Do not rely on the default constructor
                            decltype(T{} < T{})>> : std::true_type
{
};

template <typename T> constexpr auto is_comparable_v = is_comparable<T>::value;

template <typename T, bool is_pod = std::is_pod_v<T>> class IUnique : public less_than_comparable<IUnique<T>>
{
public:
  using Key = T;
  static_assert(std::is_same_v<std::remove_cv_t<Key>, Key>, "Key must be a non-const, non-volatile type");

public:
  static_assert(is_comparable_v<T>); // true

  IUnique() = default;
  virtual ~IUnique() = default;

public:
  template <typename U> friend bool operator<(const IUnique<U> &lhs, const IUnique<U> &rhs);

public:
  virtual const T &GetUnique() const = 0;
};

template <typename T>
// POD types do not have a default constructor
class IUnique<T, true> : public less_than_comparable<IUnique<T>>
{

public:
  using Key = T;

  static_assert(std::is_same<T, std::remove_cv_t<T>>::value, "POD types must be the same type");

public:
  IUnique() = default;
  virtual ~IUnique() = default;

  template <typename U> friend bool operator < (const IUnique<U> &lhs, const IUnique<U> &rhs);

public:
  virtual T GetUnique() const = 0;
private:

};

template <typename U> bool operator<(const IUnique<U> &lhs, const IUnique<U> &rhs)
{
  return lhs.GetUnique() < rhs.GetUnique();
}

// static_assert(is_comparable_v<IUnique<std::string>>);       // true
// static_assert(is_comparable_v<IUnique<std::vector<char>>>); // true
// static_assert(is_comparable_v<IUnique<int>>);               // true
// static_assert(is_comparable_v<IUnique<std::string &>>);     // false
// static_assert(std::is_base_of_v<less_than_comparable<IUnique<int>>, IUnique<int>>);
// static_assert(std::is_pod_v<uint64_t>); // true

using StringUnique = IUnique<std::string>;
using IntUnique = IUnique<int>;
using Uint64TUnique = IUnique<uint64_t>;
using Uint32TUnique = IUnique<uint32_t>;
using Uint16TUnique = IUnique<uint16_t>;
using Uint8TUnique = IUnique<uint8_t>;
using CharUnique = IUnique<char>;
using BoolUnique = IUnique<bool>;
