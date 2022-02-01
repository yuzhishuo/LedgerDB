/*
 * @Author: Yimin Liu
 * @Date: 2022-01-21 16:46:09
 * @LastEditTime: 2022-02-01 23:10:51
 * @LastEditors: Yimin Liu
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /LedgerDB/test/main.cpp
 */
#include <gtest/gtest.h>

#include <common/SelfDictionary.h>

struct Unique : public IUnique<int> {
  virtual int GetUnique() const override { return id_; }
  void setId(int id) { id_ = id; }

private:
  int id_;
};

class SelfDictionaryTest : public ::testing::TestWithParam<int> {};

INSTANTIATE_TEST_CASE_P(TrueReturn, SelfDictionaryTest,
                        testing::Values(3, 5, 11, 23, 17, 10, 12));

yuzhi::SelfDictionary<Unique> dictionary;
TEST_P(SelfDictionaryTest, ADDTrueReturn) {
  int n = GetParam();

  Unique unique;
  unique.setId(n);

  if (unique.GetUnique() % 2) {
    auto error = dictionary.Add(unique);
    ASSERT_FALSE(error);
  } else {
    auto error = dictionary.Add(unique);
    ASSERT_FALSE(error);
    auto error1 = dictionary.Add(unique);
    ASSERT_TRUE(error1);
  }
}

TEST_P(SelfDictionaryTest, RemoveTrueReturn) {
  int n = GetParam();

  Unique unique;
  unique.setId(n);

  if (unique.GetUnique() % 2) {
    auto error = dictionary.Remove(unique);
    ASSERT_FALSE(error);
  } else {
    auto error = dictionary.Remove(unique);
    ASSERT_FALSE(error);
    auto error1 = dictionary.Remove(unique);
    ASSERT_TRUE(error1);
  }
}

