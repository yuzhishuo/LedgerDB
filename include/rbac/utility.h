#pragma once
#include <utility>

#ifndef AuthorityCertificationHelper
#define AuthorityCertificationHelper(name)                                     \
  AuthorityCertification::Instance().RegisterStrategy(                         \
      std::make_unique<name##Strategy>())
#endif
