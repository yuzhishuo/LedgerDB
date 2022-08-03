/*
 * @Author: Leo
 * @Date: 2022-08-01 01:59:11
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-01 02:12:43
 */
#ifndef LEDGERDB_YUZHI_SERVER__INTERFACE_H
#define LEDGERDB_YUZHI_SERVER__INTERFACE_H
#include <common/common.h>
#include <optional>
#include <string>
#include <utility>
#include <vector>
namespace yuzhi::server::interface
{
class ILedger
{
public:
  // Read, get 你存的东西 ，就要一个参数 jsn
  std::pair<std::vector<uint8_t>, std::optional<common::Error>> Read(const std::string &jsn);
  // Verify 1001 和 1000 来判断是否是合法的
  std::optional<common::Error> Verify(const std::string &jsn, std::vector<uint8_t> &data);
  // Write  set 你存的东西 ，就要一个参数 一个返回值 root hash ,jsn
  // 是一个叶子节点的id office
  std::pair<std::string, std::optional<common::Error>> Write(const std::vector<uint8_t> &content);
  // Grant
  std::optional<common::Error> Grant(const std::string &id);
  // Delete
  std::optional<common::Error> Delete();
  // Purge
  std::optional<common::Error> Purge();
  // Hide
  std::optional<common::Error> Hide();
  virtual ~ILedger() = default;
};
} // namespace yuzhi::server::interface

#endif