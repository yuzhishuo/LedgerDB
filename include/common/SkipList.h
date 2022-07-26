/*
 * @Author: Leo
 * @Date: 2022-07-25 05:56:39
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-26 14:05:54
 */
#ifndef YUZHI_COMMON_SKIP_LIST
#define YUZHI_COMMON_SKIP_LIST
#include <assert.h>
#include <optional>
#include <stdlib.h>
#include <vector>
namespace yuzhi::common
{
namespace
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
} // namespace
class SkipList
{
public:
  SkipList() : head_{nullptr}, tail_{nullptr}
  {
    auto node = new ListLayer;
    head_ = node;
    tail_ = node;
  }
  ~SkipList()
  {
    assert(level_ < max_level_);
    for (auto layer = tail_; layer; layer = static_cast<ListLayer *>(layer->down))
    {
      while (auto node = layer->next)
      {
        layer->next = node->next;
        if (layer == head_)
          delete node->store;
        delete node;
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
   *
   *
   * @return true if the key is present in the list
   * @return false  no key is present in the list
   */
  bool remove(int key)
  {
    std::vector<List *> update(max_level_, nullptr);
    {

      auto node = static_cast<ListLayer *>(tail_);
      for (int i = level_; i > 0; i--)
      {
        while (node->next != nullptr && node->next->store->first < key)
        {
          node = static_cast<ListLayer *>(node->next);
        }
        update[i] = node;
        if (i > 1)
          node = static_cast<ListLayer *>(node->down);
      }
    }

    auto node = update[1]->next;
    if (node && node->store->first == key)
    {
      for (auto i = 2; i <= level_; i++)
      {
        if (update[i]->next && update[i]->next->store->first == key)
        {
          auto delete_node = update[i]->next;
          update[i]->next = delete_node->next;
          delete delete_node;
          continue;
        }
        break;
      }
      auto delete_node = node;
      update[1]->next = delete_node->next;
      delete delete_node->store;
      delete delete_node;

      while (level_ > 1 and !tail_->next)
      {
        auto delete_list_layer = tail_;
        tail_ = static_cast<ListLayer *>(tail_->down);
        delete delete_list_layer;
        level_++;
      }

      return true;
    }
    return false;
  }
  /**
   * @param key  find key
   *
   * @return std::optional<int> find value
   */
  std::optional<int> find(int key) const
  {
    auto node = static_cast<ListLayer *>(tail_);
    for (int i = level_; i > 0; i--)
    {
      while (node->next != nullptr && node->next->store->first < key)
      {
        node = static_cast<ListLayer *>(node->next);
      }
      if (node->next && node->next->store->first == key)
      {
        return node->next->store->second;
      }
      if (i > 1)
        node = static_cast<ListLayer *>(node->down);
    }
    return std::nullopt;
  }

  /**
   * @param value the value to insert
   *
   * @return true if the value is inserted, false if the value is already in the list
   */
  bool insert(int key, int val)
  {
    std::vector<List *> update(max_level_ + 1, nullptr);

    {
      auto node = static_cast<ListLayer *>(tail_);
      for (int i = level_; i > 0; i--)
      {
        while (node->next != nullptr && node->next->store->first < key)
        {
          node = static_cast<ListLayer *>(node->next);
        }
        update[i] = node;
        if (i > 1)
          node = static_cast<ListLayer *>(node->down);
      }
    }

    auto node = update[1]->next;

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
      for (auto i = 2; i <= level_; i++)
      {
        auto new_layer = new ListLayer(update[i - 1]->next);
        new_layer->store = new_node->store;
        if (update[i]->next)
        {
          new_layer->next = update[i]->next->next;
        };
        update[i]->next = new_layer;
      }
    }
    return true;
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
}; // namespace yuzhi::common

} // namespace yuzhi::common

#endif // YUZHI_COMMON_SKIP_LIST