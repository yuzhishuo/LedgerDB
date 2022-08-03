/*
 * @Author: Leo
 * @Date: 2022-08-03 03:59:32
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-03 14:40:09
 */
#include <utility/Snow.h>

#include <chrono>
#include <gtest/gtest.h>

#include <limits.h>

TEST(SNOW, test1)
{
  constexpr auto test = std::numeric_limits<uint64_t>::max() ^ (std::numeric_limits<uint64_t>::max() << 22);
  static_assert(test == 0b0011'1111'1111'1111'1111'1111uLL);
}

TEST(SNOW, test2)
{
  using namespace yuzhi::utility;
  [[maybe_unused]] struct SnowflakeIdWorker snowflakeIdWorker;

  snowflakeIdWorkerInit(&snowflakeIdWorker, 3, 1);

  auto mutex = std::mutex();
  [[maybe_unused]] auto new_id = nextId(&snowflakeIdWorker, mutex);

  [[maybe_unused]] auto new_id2 = nextId(&snowflakeIdWorker, mutex);

  EXPECT_LT(new_id, new_id2);
}