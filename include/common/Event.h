/*
 * @Author: Leo
 * @Date: 2022-08-10 01:14:47
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-10 01:38:45
 */

#pragma once

#ifndef LEDGERDB_YUZHI_COMMON_ERROR_H
#define LEDGERDB_YUZHI_COMMON_ERROR_H

#include "Error.h"
#include <any>
#include <functional>
#include <optional>
#include <string>

namespace yuzhi::common
{

class Event
{
public:
  Event(const std::string &event_name) : event_name_(event_name){};

  void register_(std::function<void(const std::any &)> callback) { callback_.swap(callback); }
  std::optional<Error> fire_(std::any &arg)
  {

    if (callback_)
    {
      try
      {
        callback_(arg);
      }
      catch (const std::exception &e)
      {
        return Error(e.what());
      }
      catch (...)
      {
        return Error("Unknown exception");
      }
    }
    return std::nullopt;
  }

private:
  std::string event_name_;
  std::function<void(const std::any &)> callback_;
};

} // namespace yuzhi::common

#endif // LEDGERDB_YUZHI_COMMON_ERROR_H