/*
 * @Author: Leo
 * @Date: 2022-07-26 06:24:06
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-26 06:55:32
 */
#ifndef LEDGER_YUZHI_COMMON_LRUCACHE_H
#define LEDGER_YUZHI_COMMON_LRUCACHE_H

#pragma onece
#include <unordered_map>

namespace yuzhi::common
{

namespace
{

template <typename T> struct TwoWayLinkList
{
  TwoWayLinkList *prev;
  TwoWayLinkList *next;
  T *data;
};

class LRUCache
{

public:
  LRUCache(int capacity) : capacity_(capacity), size_(0)
  {

    head_ = new TwoWayLinkList<std::pair<int, int>>;
    tail_ = new TwoWayLinkList<std::pair<int, int>>;
    head_->next = tail_;
    head_->prev = tail_;

    tail_->prev = head_;
    tail_->next = head_
  }

private:
  int capacity_;
  int size_;
  TwoWayLinkList<std::pair<int, int>> *head_;
  TwoWayLinkList<std::pair<int, int>> *tail_;
  std::unordered_map<int, TwoWayLinkList<std::pair<int, int> *>> map_;
};

} // namespace
} // namespace yuzhi::common

} // namespace yuzhi::common

#endif // LEDGER_YUZHI_COMMON_LRUCACHE_H