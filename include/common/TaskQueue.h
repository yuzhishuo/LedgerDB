
#pragma once
#ifndef LEDGER_YUZHI_COMMON_TASKQUEUE_H_
#define LEDGER_YUZHI_COMMON_TASKQUEUE_H_

#include <condition_variable>
#include <mutex>
#include <vector>
namespace yuzhi::common
{

template <typename T> class BlockingQueue
{

public:
  BlockingQueue(const BlockingQueue<T> &other) = delete;
  BlockingQueue<T> &operator=(const BlockingQueue<T> &other) = delete;
  BlockingQueue(int capacity) : capacity_(capacity), _vt(capacity + 1), start_(0), end_(0) {}

  bool isempty() { return end_ == start_; }

  bool isfull() { return (start_ + capacity_ - end_) % (capacity_ + 1) == 0; }

  void push(const T &e)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    while (isfull())
    {
      not_full_.wait(lock);
    }

    _vt[end_++] = e;
    end_ %= (capacity_ + 1);
    not_empty_.notify_one();
  }

  T pop()
  {
    std::unique_lock<std::mutex> lock(mutex_);
    while (isempty())
    {
      not_empty_.wait(lock);
    }

    auto res = _vt[start_++];
    start_ %= (capacity_ + 1);
    not_full_.notify_one();
    return res;
  }

private:
  std::mutex mutex_;
  std::condition_variable not_full_;
  std::condition_variable not_empty_;
  int start_;
  int end_;
  int capacity_;
  std::vector<T> _vt;
};

} // namespace yuzhi::common

#endif