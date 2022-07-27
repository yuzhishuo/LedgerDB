/*
 * @Author: Leo
 * @Date: 2022-07-26 02:19:18
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-27 02:59:36
 */
#include <common/SkipList.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <vector>
using namespace yuzhi::common;

struct SkipListBaseTest : public ::testing::TestWithParam<int>
{

  static std::vector<int> flags;
  static SkipList<int, int> list;
};

SkipList<int, int> SkipListBaseTest::list;
std::vector<int> SkipListBaseTest::flags(1'001, 0);
static constexpr auto val = 3;
TEST_P(SkipListBaseTest, INSERT)
{
  int n = GetParam();
  SkipListBaseTest::list.insert(n, val);

  auto r = (random() % n) + 1;

  EXPECT_TRUE(SkipListBaseTest::list.find(n).value() == val);

  EXPECT_TRUE(SkipListBaseTest::list.remove(r) | flags[r]);
  flags[r] = 1;
  EXPECT_TRUE((!SkipListBaseTest::list.find(r).has_value()) & flags[r]);
}

INSTANTIATE_TEST_CASE_P(SkipList_integer_Preparation, SkipListBaseTest, ::testing::Range(1, 1'000));

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

TEST(SkipListTest, GENERIC)
{
  SkipList<std::string, int> list;

  list.insert("a", val);
  list.insert("b", val);
  list.insert("c", val);
  list.insert("d", val);
  list.insert("e", val);
  ASSERT_TRUE(list.find("a").value() == val);

  ASSERT_TRUE(list.remove("a"));

  list.insert("d", val);
  ASSERT_TRUE(list.remove("d"));
  ASSERT_FALSE(list.remove("d"));
}