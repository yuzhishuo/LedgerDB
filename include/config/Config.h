/*
 * @Author: Leo
 * @Date: 2022-01-30 19:06:00
 * @LastEditTime: 2022-02-28 17:00:49
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/config/Config.h
 */

#pragma once

#include <rude/config.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>

#include <config/IConfigurable.hpp>

namespace yuzhi {

class Config {
 public:
  Config(std::string config_file_path) {
    if (!config_.load(config_file_path.data())) {
      spdlog::error("load config file failed");
      exit(1);
    }
  }

  template <typename T>
  T get(const IConfigurable &configurable, const std::string &key) {
    bool expected = false;
    while (!loaded.compare_exchange_weak(
        expected, true, std::memory_order_acquire, std::memory_order_release))
      std::this_thread::yield();

    config_.setSection(configurable.Field(), false);
    auto res = this->_get<T>(key);
    loaded.store(false, std::memory_order_release);
    return res;
  }

  template <typename T>
  T get(const IConfigurable *configurable, const std::string &key) {
    bool expected = false;
    while (!loaded.compare_exchange_weak(
        expected, true, std::memory_order_acquire, std::memory_order_release))
      std::this_thread::yield();

    config_.setSection(configurable->Field(), false);

    auto res = this->_get<T>(key);
    loaded.store(false, std::memory_order_release);
    return res;
  }

 private:
  template <typename T>
  inline T _get(const std::string &key) {
    throw std::runtime_error("not implemented");
  }

 public:
  static Config &Instance() {
    static Config instance{"config.default.conf"};
    return instance;
  }

 private:
  std::atomic<bool> loaded{false};
  rude::Config config_;
};

template <>
inline int Config::_get<int>(const std::string &key) {
  return config_.getIntValue(key.data());
}
template <>
inline double Config::_get<double>(const std::string &key) {
  return config_.getDoubleValue(key.data());
}

template <>
inline bool Config::_get<bool>(const std::string &key) {
  return config_.getBoolValue(key.data());
}
template <>
inline std::string Config::_get<std::string>(const std::string &key) {
  return config_.getStringValue(key.data());
}
}  // namespace yuzhi