/*
 * @Author: Leo
 * @Date: 2022-08-10 03:55:41
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-18 12:53:10
 */

#include <gtest/gtest.h>
#include <rxcpp/rx.hpp>

class Fire
{
  void range(int begin, int end)
  {
    auto values = rxcpp::observable<>::range(begin, end).filter([](int v) { return v % 2 == 0; }).map([](int x) {
      return x * x;
    });
    values.subscribe([](int v) { std::cout << v << std::endl; });
  }
};

TEST(RXCPP, TEST1)
{
  // TODO: test
}