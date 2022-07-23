/*
 * @Author: Leo
 * @Date: 2022-07-22 09:36:21
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-23 16:42:24
 */
#pragma once
#include <utility>

#ifndef AuthorityCertificationHelper
#define AuthorityCertificationHelper(name)                                                                             \
  AuthorityCertification::Instance().RegisterStrategy(std::make_unique<name##Strategy>())
#endif
