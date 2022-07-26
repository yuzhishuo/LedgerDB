/*
 * @Author: Leo
 * @Date: 2022-07-22 09:08:07
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-26 02:35:34
 */

#include <common/SelfDictionary.h>
#include <gtest/gtest.h>

struct Unique : public IUnique<int>
{
  virtual int GetUnique() const override { return id_; }
  void setId(int id) { id_ = id; }

private:
  int id_;
};

class SelfDictionaryTest : public ::testing::TestWithParam<int>
{
};

INSTANTIATE_TEST_CASE_P(TrueReturn, SelfDictionaryTest, testing::Values(3, 5, 11, 23, 17, 10, 12));

yuzhi::SelfDictionary<Unique> dictionary;
TEST_P(SelfDictionaryTest, ADDTrueReturn)
{
  int n = GetParam();

  Unique unique;
  unique.setId(n);

  if (unique.GetUnique() % 2)
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

  if (unique.GetUnique() % 2)
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
