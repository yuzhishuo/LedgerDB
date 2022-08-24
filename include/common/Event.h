/*
 * @Author: Leo
 * @Date: 2022-08-10 01:14:47
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-21 00:23:11
 */

#pragma once

#ifndef LEDGERDB_YUZHI_COMMON_ERROR_H
#define LEDGERDB_YUZHI_COMMON_ERROR_H

#include "Error.h"

#include <assert.h>
#include <functional>
#include <optional>
#include <rxcpp/rx.hpp>
#include <spdlog/spdlog.h>
#include <string>
namespace yuzhi::common
{
class Subscription final
{
public:
  Subscription(std::shared_ptr<rxcpp::composite_subscription> subscription) : subscription_(subscription)
  {
    assert(subscription);
  }

  void cancel() { subscription_.lock()->unsubscribe(); }

private:
  std::weak_ptr<rxcpp::composite_subscription> subscription_;
};

// template<typename T>
class Event final
{
public:
  Event(const std::string &event_name) : event_name_(event_name) { update(); };

  void update()
  {
    subscriber_ = std::make_unique<rxcpp::subscriber<int>>(rxcpp::make_subscriber<int>(
        *subscription_, std::bind(subscription_callback, Subscription{subscription_}, std::placeholders::_1),
        [&]() { std::bind(finish_callback, Subscription{subscription_})(); }));
  }

  void crash(int v) { subscriber_->on_next(v);  update(); }

#pragma region fire
public: // fire
  class Fire final
  {
  public:
    Fire(Event *event) : event_{event} {}
    void setCallback(std::function<void(Subscription, int)> next) { event_->setNext(next); }

  private:
    Event *event_;
  };
  std::shared_ptr<Fire> fire()
  {
    if (!fire_)
    {
      fire_ = std::make_unique<Fire>(this);
    }
    return fire_;
  }
#pragma endregion
private:
  void setNext(std::function<void(Subscription, int)> next) { subscription_callback = std::move(next); }

private:
  std::string event_name_;
  std::unique_ptr<rxcpp::subscriber<int>> subscriber_;
  std::shared_ptr<rxcpp::composite_subscription> subscription_ = std::make_shared<rxcpp::composite_subscription>();
  std::function<void(Subscription, int)> subscription_callback = [](Subscription, int) { SPDLOG_WARN("no callback"); };
  std::function<void(Subscription)> finish_callback = [](Subscription) { SPDLOG_WARN("no finish callback"); };
  std::shared_ptr<Fire> fire_;
};

} // namespace yuzhi::common

#endif // LEDGERDB_YUZHI_COMMON_ERROR_H