/*
 * @Author: Leo
 * @Date: 2022-07-20 08:29:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-21 07:28:51
 */
#pragma once
#include <filesystem>
#include <fmt/core.h>

namespace yuzhi::meta {

static constexpr auto KLedgerStoreName = "ledgerdb";

static constexpr auto KLedgerStoreVersion = 1.0;

static constexpr auto KLedgerStoreKey = "ledger_db_{}";

static constexpr auto KLedgerColumnFamilyName = "ledger_column_family";

static constexpr auto KUserColumnFamilyName = "user_column_family";

static constexpr auto KLedgerStoreKeyCurrentLedgerId = "current_ledger_id";

static constexpr auto KLedgerStoreKeyCurrentUserId = "current_user_id";

static constexpr auto KLedgerUserStoreKey = "ledger_db_user_{}_{}";

inline std::string genLedgerDBStoreRootPath() noexcept {
  using namespace std::filesystem;
  return current_path()
      .concat(fmt::format("{}/{}", KLedgerStoreName,
                          std::to_string(KLedgerStoreVersion)))
      .string();
}

inline std::string genLedgerDBStorePath(std::string db_name) noexcept {
  return fmt::format("{}/{}", genLedgerDBStoreRootPath(), db_name);
}

inline std::string genUserStoreOfKey(const std::string &ledger_name,
                                     const std::string &user_name) noexcept {
  return fmt::format(KLedgerUserStoreKey, ledger_name, user_name);
}

inline std::string
genLedgerStoreOfKey(const std::string &ledger_name) noexcept {
  return fmt::format(KLedgerStoreKey, ledger_name);
}

inline std::string genCurrentLedgerIdOfKey() noexcept {
  return KLedgerStoreKeyCurrentLedgerId;
}

inline std::string genCurrentUserIdOfKey() noexcept {
  return KLedgerStoreKeyCurrentUserId;
}

inline std::string genLedgerColumnFamilyName() noexcept {
  return KLedgerColumnFamilyName;
}

inline std::string genUserColumnFamilyName() noexcept {
  return KUserColumnFamilyName;
}

} // namespace yuzhi::meta