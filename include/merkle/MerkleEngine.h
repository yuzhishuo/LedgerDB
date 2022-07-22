/*
 * @Author: Leo
 * @Date: 2022-01-25 16:36:55
 * @LastEditTime: 2022-07-22 09:48:46
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/merkle/MerkleEngine.h
 */
#pragma once
#include <memory>
#include <string>

#include <optional>

#include <common/Error.h>
#include <merkle/merklecpp/merklecpp.h>
#include <merkle_engine.pb.h>
#include <rocksdb/db.h>

namespace yuzhi
{
class Ledger;
const std::string kMerkleRoot = "/tmp/merkle/";
constexpr int kMerkleTreeSize = 64;
class MerkleEngine final
{

public:
  MerkleEngine(std::shared_ptr<Ledger> ledger);
  ~MerkleEngine();

public:
  /**
   * @brief add a node to merkle tree, and return the root hash
   *
   * @param jsn leaf node hash
   * @return std::pair<merkle_engine::LeafMeta, std::optional<common::Error>>
   * leaf meta and error
   */
  std::pair<merkle_engine::LeafMeta, std::optional<common::Error>> Add(const std::string &hash)
  {
    auto index = merkle_->num_leaves() + 1;

    merkle_engine::LeafMeta leaf_meta;
    leaf_meta.set_hash(hash);
    leaf_meta.set_id(merkle_id);
    leaf_meta.set_index(index);

    std::vector<uint8_t> root_hash;

    merkle_->insert(hash);

    if (is_full())
    {
      merkle_->serialise(root_hash);
      if (rocksdb::Status status =
              db_->Put(rocksdb::WriteOptions(), kMerkleRoot, std::string(root_hash.begin(), root_hash.end()));
          !status.ok())
      {
        return {{}, common::Error::MerkleTreeUpdateError()};
      }
      // 清空
      merkle_.reset(new merkle::Tree());
    }

    return {leaf_meta, {}};
  }
  std::optional<common::Error> Compress(const std::string &jsn);

  // for test
  std::optional<common::Error> Delete(const std::string &jsn);

private:
  bool is_full() { return merkle_->num_leaves() == kMerkleTreeSize; }

private:
  uint64_t merkle_id = 0;
  std::string tree_store_path_;
  std::shared_ptr<Ledger> ledger_;
  std::unique_ptr<merkle::Tree> merkle_;
  rocksdb::DB *db_;
};
} // namespace yuzhi