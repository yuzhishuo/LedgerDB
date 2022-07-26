//
// Created by yuzhi on 5/25/22.
//
#pragma once
#ifndef LEDGERDB_FACTOR_H
#define LEDGERDB_FACTOR_H

namespace yuzhi::utility
{
template <typename FactorClass> class Factor
{

public:
  template <typename... Arg> static auto getInstance(Arg... arg) { return FactorClass::instance(); }
};
} // namespace yuzhi::utility

#endif // LEDGERDB_FACTOR_H
