/*
 * @Author: Leo
 * @Date: 2022-07-22 09:08:07
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-21 15:09:12
 */

#include <common/SelfDictionary.h>
#include <gtest/gtest.h>

struct Unique : public IUnique<int>
{
  int getUnique() const override { return id_; }
  void setId(int id) { id_ = id; }

private:
  int id_;
};

class SelfDictionaryTest : public ::testing::TestWithParam<int>
{
};

INSTANTIATE_TEST_CASE_P(TrueReturn, SelfDictionaryTest, testing::Values(3, 5, 11, 23, 17, 10, 12));

static yuzhi::SelfDictionary<Unique> dictionary;

TEST_P(SelfDictionaryTest, ADDTrueReturn)
{
  int n = GetParam();

  Unique unique;
  unique.setId(n);

  if (unique.getUnique() % 2)
  {
    auto error = dictionary.Add(unique);
    ASSERT_FALSE(error);
  }
  else
  {
    auto error = dictionary.Add(unique);
    ASSERT_FALSE(error);
    auto error1 = dictionary.Add(unique);
    ASSERT_TRUE(error1);
  }
}

TEST_P(SelfDictionaryTest, RemoveTrueReturn)
{
  int n = GetParam();

  Unique unique;
  unique.setId(n);

  if (unique.getUnique() % 2)
  {
    auto error = dictionary.Remove(unique);
    ASSERT_FALSE(error);
  }
  else
  {
    auto error = dictionary.Remove(unique);
    ASSERT_FALSE(error);
    auto error1 = dictionary.Remove(unique);
    ASSERT_TRUE(error1);
  }
}
