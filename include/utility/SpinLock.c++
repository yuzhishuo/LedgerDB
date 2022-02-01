/*
 * @Author: Leo
 * @Date: 2022-01-30 23:02:01
 * @LastEditTime: 2022-02-01 16:38:20
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/utility/SpinLock.c++
 */

#pragma once
#include <atomic>
namespace yuzhi::utility {
struct SpinLock {
  void lock() {
    bool expected = false;
    while (!state.compare_exchange_weak(
        expected, true, std::memory_order_acquire, std::memory_order_relaxed)) {
      expected = false;
    }
  }

  void unlock() { state.store(false, std::memory_order_release); }

private:
  std::atomic_bool state;
};
} // namespace yuzhi::utility