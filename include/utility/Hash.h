/*
 * @Author: Leo
 * @Date: 2022-08-06 08:44:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-06 13:56:23
 */
/*
 * @Author: Leo
 * @Date: 2022-08-06 08:44:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-06 12:21:29
 */
#pragma once
#ifndef LEDGERDB_YUZHI_HASH_H
#define LEDGERDB_YUZHI_HASH_H

#include <any>
#include <common/Error.h>
#include <exception>
#include <optional>
#include <string>
#include <string_view>

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/ssl.h>

namespace yuzhi::utility
{

struct HashType
{
  struct SHA256;
  struct SHA512;
  struct MD5;
}; // namespace HashType

class IDigest
{
public:
  virtual std::optional<common::Error> add(std::string_view view);
  virtual std::pair<std::optional<common::Error>, std::string> final();

protected:
};

template <typename T> class Hash
{
public:
  Hash() { throw "not implemented"; }
};

template <> class Hash<HashType::MD5> : public IDigest
{
public:
  Hash() { MD5_Init(&ctx); }
  std::optional<common::Error> add(std::string_view view) override
  {
    MD5_Update(&ctx, view.data(), view.size());
    return std::nullopt;
  };

  std::pair<std::optional<common::Error>, std::string> final() override
  {
    unsigned char MD5result[MD5_DIGEST_LENGTH] = {0};

    MD5_Final(MD5result, &ctx);

    return {std::nullopt, std::string(reinterpret_cast<char *>(MD5result), MD5_DIGEST_LENGTH)};
  };

private:
  MD5_CTX ctx;
};

template <> class Hash<HashType::SHA512> : public IDigest
{
public:
  Hash() { SHA512_Init(&ctx); }
  std::optional<common::Error> add(std::string_view view) override
  {
    SHA512_Update(&ctx, view.data(), view.size());
    return std::nullopt;
  }
  std::pair<std::optional<common::Error>, std::string> final() override
  {
    unsigned char SHA512result[SHA256_DIGEST_LENGTH] = {0};

    SHA512_Final(SHA512result, &ctx);

    return {std::nullopt, std::string(reinterpret_cast<char *>(SHA512result), SHA512_DIGEST_LENGTH)};
  };

private:
  SHA512_CTX ctx;
};

template <> class Hash<HashType::SHA256> : public IDigest
{
public:
  Hash() { SHA256_Init(&ctx); }
  std::optional<common::Error> add(std::string_view view) override
  {
    SHA256_Update(&ctx, view.data(), view.size());
    return std::nullopt;
  }
  std::pair<std::optional<common::Error>, std::string> final() override
  {
    unsigned char SHA256result[SHA256_DIGEST_LENGTH] = {0};

    SHA256_Final(SHA256result, &ctx);

    return {std::nullopt, std::string(reinterpret_cast<char *>(SHA256result), SHA256_DIGEST_LENGTH)};
  };

private:
  SHA256_CTX ctx;
}; // namespace yuzhi::utility

} // namespace yuzhi::utility

#endif // LEDGERDB_YUZHI_HASH_H