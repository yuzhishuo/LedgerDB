/*
 * @Author: Leo
 * @Date: 2022-07-26 06:24:06
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-27 02:32:48
 */
#ifndef LEDGER_YUZHI_COMMON_LRUCACHE_H
#define LEDGER_YUZHI_COMMON_LRUCACHE_H

#pragma onece
#include <assert.h>
#include <cstddef>
#include <optional>
#include <unordered_map>
#include <utility>

namespace yuzhi::common
{

template <typename T, typename U> struct DLinkedNode
{
  T key;
  U value;
  DLinkedNode<T, U> *prev;
  DLinkedNode<T, U> *next;
  DLinkedNode() : key{}, value{}, prev(nullptr), next(nullptr) {}
  DLinkedNode(T _key, U _value) : key(std::move(_key)), value(std::move(_value)), prev(nullptr), next(nullptr) {}
};

template <typename T = int, typename U = int> class LRUCache
{
private:
  using Node = DLinkedNode<T, U>;

private:
  std::unordered_map<T, Node *> cache;
  Node *head;
  Node *tail;
  std::size_t size;
  std::size_t capacity;

public:
  LRUCache() = delete;
  LRUCache(std::size_t _capacity) : head{new Node}, tail{new Node}, capacity(_capacity), size(0)
  {
    assert(_capacity > 0);
    head->next = tail;
    tail->prev = head;
  }

  ~LRUCache()
  {
    while (head != nullptr)
    {
      auto node = head;
      head = head->next;
      delete node;
    }
  }
  std::optional<U> get(T key)
  {
    if (!cache.count(key))
    {
      return std::nullopt;
    }
    Node *node = cache[key];
    moveToHead(node);
    return node->value;
  }

  void put(T key, U value)
  {
    if (!cache.count(key))
    {
      // 如果 key 不存在，创建一个新的节点
      auto *node = new Node(std::move(key), std::move(value));
      // 添加进哈希表
      cache[key] = node;
      // 添加至双向链表的头部
      addToHead(node);
      ++size;
      if (size > capacity)
      {
        // 如果超出容量，删除双向链表的尾部节点
        Node *removed = removeTail();
        // 删除哈希表中对应的项
        cache.erase(removed->key);
        // 防止内存泄漏
        delete removed;
        --size;
      }
    }
    else
    {
      Node *node = cache[key];
      node->value = value;
      moveToHead(node);
    }
  }

  void addToHead(Node *node)
  {
    node->prev = head;
    node->next = head->next;
    head->next->prev = node;
    head->next = node;
  }

  void removeNode(Node *node) const
  {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  void moveToHead(Node *node)
  {
    removeNode(node);
    addToHead(node);
  }

  Node *removeTail()
  {
    Node *node = tail->prev;
    removeNode(node);
    return node;
  }
};

} // namespace yuzhi::common

#endif // LEDGER_YUZHI_COMMON_LRUCACHE_H