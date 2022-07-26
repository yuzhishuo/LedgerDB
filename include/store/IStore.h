/*
 * @Author: Leo
 * @Date: 2022-02-01 21:47:19
 * @LastEditTime: 2022-07-22 09:51:02
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/store/IStore.h
 */
#pragma once

#include <common/Error.h>
#include <optional>
#include <string>
#include <vector>

namespace yuzhi::store
{
class IOnlyReadStore
{
public:
  virtual ~IOnlyReadStore() = default;
  virtual std::pair<std::string, std::optional<common::Error>> load(const std::string &key) = 0;
};

class IStorage : public IOnlyReadStore
{

public:
  virtual ~IStorage() = default;
  virtual std::optional<yuzhi::common::Error> save(const std::string &key, const std::string &value) = 0;
  virtual std::optional<yuzhi::common::Error> save(const std::vector<std::pair<std::string, std::string>> &kvs) = 0;
  virtual std::optional<yuzhi::common::Error> delete_key(const std::string &key) = 0;
  virtual std::optional<yuzhi::common::Error> update_key(const std::string &key, const std::string &value) = 0;
};

class ISoftStorage : public IStorage
{

public:
  virtual ~ISoftStorage() = default;
  virtual std::optional<yuzhi::common::Error> delete_key[[noreturn]](const std::string &key) override final
  {
    throw std::runtime_error("not support");
  }
};

} // namespace yuzhi::store