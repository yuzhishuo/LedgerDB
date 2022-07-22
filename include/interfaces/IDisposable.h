/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 05:40:33
 */
#pragma once

namespace yuzhi::interface {

class IDisposable {
public:
  virtual void dispose() = 0;
  virtual ~IDisposable() = default;
};

} // namespace yuzhi::interface
