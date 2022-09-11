/*
 * @Author: Leo
 * @Date: 2022-09-10 09:25:55
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-11 02:59:03
 */
#ifndef YUZHI_COMMON_LOOPQUEUE_
#define YUZHI_COMMON_LOOPQUEUE_

#include <memory>
#include <set>
#include <stddef.h>
#include <type_traits>

namespace yuzhi::common
{

namespace
{

template <typename T> struct LinkNode
{

  static_assert(std::is_default_constructible_v<T>, "LinkNode requires default constructible");
  std::set<T> val;
  LinkNode *next;
};

} // namespace

template <typename T> struct has_hash_specific
{
  template <typename U> static auto test(int) -> decltype(std::declval<std::hash<U>>().operator()(std::declval<U>()));
  template <typename U> static void test(...);
  enum
  {
    value = !std::is_void<decltype(test<T>(0))>::value
  };
};

template <typename T> class TimeWheel final
{

  static_assert(has_hash_specific<std::shared_ptr<void>>::value,
                "should finish hash computation for shared pointer, or should ");

public:
  TimeWheel(size_t legth) : head_{new LinkNode<T>{.val{}, .next{nullptr}}}, tail_{head_}, length_{legth}
  {
    init_link_node();
  }
  ~TimeWheel()
  {
    while (head_)
    {
      tail_ = head_;
      head_ = head_->next;
      delete tail_;
    }
  }

private:
  void init_link_node()
  {
    for (auto len = length_; len; len--)
    {
      new_tail_link_node();
    }
  }

  void remove_head_link_node()
  {
    auto head = head_->next;
    head_->next = head->next;
    head->next = nullptr;
    delete head;
  }

  void new_tail_link_node()
  {
    tail_->next = new LinkNode<T>{.val{}, .next = nullptr};
    tail_ = tail_->next;
  }

public:
  void pop()
  {
    remove_head_link_node();
    new_tail_link_node();
  }

  bool push(T v) { return tail_->val.insert(v).second; }

private:
  LinkNode<T> *head_;
  LinkNode<T> *tail_;
  size_t length_;
};

} // namespace yuzhi::common

#endif // YUZHI_COMMON_LOOPQUEUE_