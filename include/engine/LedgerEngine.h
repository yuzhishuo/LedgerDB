/*
 * @Author: Leo
 * @Date: 2022-02-01 21:47:19
 * @LastEditTime: 2022-09-17 17:38:53
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/LedgerEngine.h
 */
#pragma once
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <common/Error.h>
#include <merkle/MerkleEngine.h>



namespace yuzhi
{

class Ledger;

class LedgerEngine
{
public:
  explicit LedgerEngine(std::weak_ptr<Ledger> ledger);
  ~LedgerEngine() = default;

public:
  // Read, get 你存的东西 ，就要一个参数 jsn
  std::optional<common::Error> Read(const std::string_view &jsn, std::vector<uint8_t> &data);
  // Verify 1001 和 1000 来判断是否是合法的
  std::optional<common::Error> Verify(const std::string &jsn, std::vector<uint8_t> &data);
  // Write  set 你存的东西 ，就要一个参数 一个返回值 root hash ,jsn
  // 是一个叶子节点的id office
  std::optional<common::Error> Write(const std::vector<uint8_t> &content, std::string &hash);
  // Grant
  std::optional<common::Error> Grant(const std::string &id);
  // Delete
  std::optional<common::Error> Delete();
  // Purge
  std::optional<common::Error> Purge();
  // Hide
  std::optional<common::Error> Hide();

private:
  std::weak_ptr<Ledger> ledger_;
  MerkleEngine merkle_engine_;
};

} // namespace yuzhi