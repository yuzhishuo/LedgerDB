/*
 * @Author: Leo
 * @Date: 2022-07-25 05:56:39
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-25 16:40:45
 */
#ifndef YUZHI_COMMON_SKIP_LIST
#define YUZHI_COMMON_SKIP_LIST
#include <optional>
#include <stdlib.h>
#include <vector>
namespace yuzhi::common
{

struct List
{
  List *next;
  std::pair<int, int> *store;

  List() : next(nullptr), store(nullptr) {}
  List(List *next, int key, int val) : next(next), store(new std::pair(key, val)) {}
  ~List() {}
};

struct ListLayer : public List
{
  List *down;

  ListLayer() : down(nullptr), List() {}
  ListLayer(List *d) : down(d), List() {}
};

class SkipList
{
public:
  SkipList() : head_{nullptr}, tail_{nullptr}
  {
    auto node = new ListLayer();
    head_ = node;
    tail_ = node;
  }
  ~SkipList()
  {
    for (auto layer = tail_; layer != nullptr; layer = static_cast<ListLayer *>(layer->down))
    {
      while (true)
      {
        if (auto node = layer->next; node)
        {
          layer->next = node->next;
          if (layer == head_)
            delete node->store;
          delete node;
        }
      }
    }

    for (auto layer = tail_; layer != nullptr; layer = static_cast<ListLayer *>(layer->down))
    {
      delete layer;
    }
  }
  /**
   * @brief
   *
   * @param key
   * @return true if the key is present in the list
   * @return false  no key is present in the list
   */
  bool remove(int key)
  {
    for (auto layer = tail_; layer != nullptr; layer = static_cast<ListLayer *>(layer->down))
    {
      auto node = static_cast<List *>(layer->next);
      while (node != nullptr && node->next->store->first <= key)
      {
        if (node->next->store->first == key)
        {
          List *delete_node = nullptr;
          while (layer != head_)
          {
            delete_node = node->next;
            node->next = delete_node->next;
            auto down = static_cast<ListLayer *>(delete_node)->down;
            delete delete_node;
            delete_node = down;
            layer = static_cast<ListLayer *>(layer->down);
          }
          delete delete_node->store;
          delete delete_node;
          return true;
        }

        node = node->next;
      }
      return false;
    }
  }
  /**
   * @param key  find key
   *
   * @return std::optional<int> find value
   */
  std::optional<int> find(int key) const {}

  /**
   * @param value the value to insert
   *
   * @return true if the value is inserted, false if the value is already in the list
   */
  bool insert(int key, int val)
  {
    std::vector<List *> update(max_level_, nullptr);

    int i = level_;
    for (auto layer = tail_; i > 0; layer = static_cast<ListLayer *>(layer->down))
    {
      auto node = static_cast<List *>(layer->next);
      while (node->next == nullptr || node->next->store->first < key)
      {
        node = static_cast<List *>(node->next);

        if (node->store->first > key && (node->next == nullptr || node->next->store->first <= key))
        {
          update[i] = node;
        }
      }
      i--;
    }

    auto node = head_->next;

    if (node != nullptr && node->store->first == key)
    {
      node->store->second = val;
    }
    else
    {
      auto level = randomLevel();
      if (level > level_)
      {
        for (auto i = level_ + 1; i <= level; i++)
        {
          newLayer();
          update[i] = tail_;
        }
        level_ = level;
      }
      auto new_node = new List(nullptr, key, val);
      new_node->next = update[1]->next;
      update[1]->next = new_node;
      for (auto i = 2; i <= level; i++)
      {

        auto new_layer = new ListLayer(update[i - 1]);
        new_layer->store = new_layer->store;
        if (update[i]->next)
        {
          new_layer->next = update[i]->next->next;
        };
        update[i]->next = new_layer;
      }
    }
  }

private:
  void newLayer() { tail_ = new ListLayer(tail_); }
  int randomLevel() const
  {
    int level = 1;
    while ((random() & 0xFFFF) < (0.25 * 0xFFFF))
      level++;

    return level < max_level_ ? level : max_level_;
  }

private:
  ListLayer *head_;
  ListLayer *tail_;
  u_int16_t max_level_ = 32;
  u_int16_t level_ = 1;
};

} // namespace yuzhi::common

#endif // YUZHI_COMMON_SKIP_LIST