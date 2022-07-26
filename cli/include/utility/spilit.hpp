/*
 * @Author: Leo
 * @Date: 2022-04-26 17:01:49
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:47:26
 */
#pragma once
#ifndef LEDGER_CLI_UTILITY_SPILIT_HPP
#define LEDGER_CLI_UTILITY_SPILIT_HPP

namespace ledger::cli::utility
{

#include <string_view>
#include <vector>

inline auto split(std::string_view s, std::string_view separator = ",")
{
  std::vector<std::string_view> tokens;
  size_t begin_pos = 0;
  while (begin_pos < s.size())
  {
    const size_t end_pos = s.find_first_of(separator, begin_pos);
    if (end_pos != std::string_view::npos)
    {
      if (end_pos > begin_pos)
      {
        tokens.emplace_back(&s[begin_pos], end_pos - begin_pos);
      }
      begin_pos = end_pos + 1;
    }
    else
    {
      tokens.emplace_back(&s[begin_pos]);
      break;
    }
  }
  return tokens;
}

} // namespace ledger::cli::utility

#endif // LEDGER_CLI_UTILITY_SPILIT_HPP