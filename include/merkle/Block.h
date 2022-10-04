/*
 * @Author: Leo
 * @Date: 2022-07-30 07:30:07
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-25 14:48:14
 */

#pragma once

#ifndef YUZHI_MERKLE_BLOCK_H
#define YUZHI_MERKLE_BLOCK_H
#include <algorithm>
#include <assert.h>
#include <memory>
#include <merkle/merklecpp/merklecpp.h>
#include <string>

namespace yuzhi
{

struct BlockNode
{
  std::string acc_hash;
  std::string prev_hash;
  BlockNode *prev;
  std::weak_ptr<::merkle::Tree> tree;
};

class Block
{
public:
  Block() : current_node_{loadLastBoldNode()}
  {

    // load_last_node
  }

  ~Block() = default;

  BlockNode *newNode(std::shared_ptr<::merkle::Tree> tree)
  {
    assert(current_node_);
    auto prev_tree = current_node_->tree.lock();
    auto node = new BlockNode;
    node->prev_hash = prev_tree->root().to_string();
    // TODO: should calculate a new hash from  prev_hash and acc_hash
    node->acc_hash = current_node_->acc_hash + node->prev_hash;
    node->tree = tree;

    std::swap(current_node_, node);
    return node;
  }

private:
  BlockNode *loadLastBoldNode() const { return nullptr; }

private:
  BlockNode *current_node_;
};

} // namespace yuzhi

#endif // YUZHI_MERKLE_BLOCK_H