/*
 * @Author: Leo
 * @Date: 2022-07-27 01:30:02
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-27 02:29:08
 */

#include <gtest/gtest.h>

#include <common/LRUCache.h>
#include <string>

using namespace yuzhi::common;

struct LRUCacheBaseTest : public ::testing::TestWithParam<int>
{
};

constexpr auto val = 4;

TEST(LRUCacheBaseOperatorTest, INT_PUT)
{
  LRUCache cache{4};
  cache.put(1, val);
  cache.put(2, val);
  cache.put(3, val);
  cache.put(4, val);
  EXPECT_TRUE(cache.get(1) == val);
  EXPECT_TRUE(cache.get(2) == val);
  EXPECT_TRUE(cache.get(3) == val);
  EXPECT_TRUE(cache.get(4) == val);
  cache.put(5, val);
  EXPECT_FALSE(cache.get(1).has_value());
  EXPECT_TRUE(cache.get(2) == val);
  cache.put(4, val);
  EXPECT_TRUE(cache.get(3) == val);
  EXPECT_TRUE(cache.get(4) == val);
  EXPECT_TRUE(cache.get(5) == val);
  cache.put(10, val);
  EXPECT_FALSE(cache.get(2).has_value());
  EXPECT_TRUE(cache.get(3) == val);
  cache.put(11, val);
  EXPECT_FALSE(cache.get(4).has_value());
}

TEST(LRUCacheBaseOperatorTest, STRING_PUT)
{
  LRUCache<std::string, int> cache{4};
  cache.put("stop", val);
  cache.put("A", val);
  cache.put("B", val);
  cache.put("C", val);
  cache.put("D", val);
  EXPECT_FALSE(cache.get("stop").has_value());
}