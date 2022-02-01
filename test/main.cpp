/*
 * @Author: your name
 * @Date: 2022-01-21 16:46:09
 * @LastEditTime: 2022-02-01 22:44:56
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/test/main.cpp
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
                        testing::Values(3, 5, 11, 23, 17));

TEST_P(SelfDictionaryTest, HandleTrueReturn) {
  int n = GetParam();

  yuzhi::SelfDictionary<Unique> dictionary;

  Unique unique;
  unique.setId(n);

  auto error = dictionary.Add(unique);
  ASSERT_FALSE(error);
}
