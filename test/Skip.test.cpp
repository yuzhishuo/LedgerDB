/*
 * @Author: Leo
 * @Date: 2022-07-26 02:19:18
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-26 06:06:07
 */
#include <common/SkipList.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <vector>
using namespace yuzhi::common;

struct SkipListFindTest : public ::testing::TestWithParam<int>
{

  static std::vector<int> flags;
  static SkipList list;
};

SkipList SkipListFindTest::list;
std::vector<int> SkipListFindTest::flags(1'001, 0);
static constexpr auto val = 3;
TEST_P(SkipListFindTest, INSERT)
{
  int n = GetParam();
  SkipListFindTest::list.insert(n, val);

  auto r = (random() % n) + 1;

  EXPECT_TRUE(SkipListFindTest::list.find(n).value() == val);

  EXPECT_TRUE(SkipListFindTest::list.remove(r) | flags[r]);
  flags[r] = 1;
  EXPECT_TRUE((!SkipListFindTest::list.find(r).has_value()) & flags[r]);
}

INSTANTIATE_TEST_CASE_P(SkipList_interger_paraper, SkipListFindTest, ::testing::Range(1, 1'000));

TEST(SkipListTest, INSERT)
{

  SkipList list;
  list.insert(1, 12);

  ASSERT_TRUE(list.find(1).value() == 12);

  list.insert(2, 13);

  ASSERT_TRUE(list.find(2).value() == 13);

  list.insert(3, 14);

  ASSERT_TRUE(list.find(3).value() == 14);

  list.insert(4, 14);

  ASSERT_TRUE(list.find(4).value() == 14);

  list.insert(5, 14);

  ASSERT_TRUE(list.find(5).value() == 14);

  list.insert(6, 14);

  ASSERT_TRUE(list.find(5).value() == 14);

  list.insert(5, 22);

  ASSERT_TRUE(list.find(5).value() == 22);
}