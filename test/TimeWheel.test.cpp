/*
 * @Author: Leo
 * @Date: 2022-09-10 15:31:34
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-11 03:14:01
 */

#include <gtest/gtest.h>

#include <common/TimeWheel.h>
#include <memory>

struct event
{
  int id = 0;
};

TEST(TimeWheel1, FunctionTEST)
{

  using yuzhi::common::TimeWheel;
  TimeWheel<std::shared_ptr<event>> timeWheel{3};

  auto e1 = std::make_shared<event>(event{.id = 1});

  timeWheel.push(e1);

  EXPECT_EQ(e1.use_count(), 2);

  auto e2 = std::make_shared<event>(event{.id = 2});
  timeWheel.push(e2);
  EXPECT_EQ(e2.use_count(), 2);
  EXPECT_EQ(timeWheel.push(e2), false);

  timeWheel.pop();
  timeWheel.pop();
  timeWheel.pop();

  EXPECT_EQ(e1.use_count(), 1);

  timeWheel.push(e1);
  EXPECT_EQ(e1.use_count(), 2);
  timeWheel.push(e1);
  EXPECT_EQ(e1.use_count(), 2);
}