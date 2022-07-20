/*
 * @Author: Leo
 * @Date: 2022-07-20 08:29:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-20 10:09:03
 */
#include <filesystem>
#include <fmt/core.h>

namespace yuzhi::meta {
namespace {
constexpr auto KLedgerStoreName = "ledgerdb";

constexpr auto KLedgerStoreVersion = 1.0;

constexpr auto KLedgerStoreKey = "ledger_db_{}";

constexpr auto KLedgerStoreKeyCurrentLedgerId = "current_ledger_id";

constexpr auto KLedgerStoreKeyCurrentUserId = "current_user_id";

constexpr auto KLedgerUserStoreKey = "ledger_db_user_{}_{}";
} // namespace

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

} // namespace yuzhi::meta