/*
 * @Author: Leo
 * @Date: 2022-02-02 00:49:30
 * @LastEditTime: 2022-02-02 01:00:42
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/test/AuthorityCertification.cpp
 */

#include <AuthorityCertification.h>
#include <gtest/gtest.h>
#include <iostream>

TEST(AuthorityCertificationT, AuthorityCertificationTEST) {

  AuthorityCertification::Instance().Start();
  std::cout << AuthorityCertification::Instance().UserPass("CreateUser",
                                                           "admin")
            << std::endl;
}