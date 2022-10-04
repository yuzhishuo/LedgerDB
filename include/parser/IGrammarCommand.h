/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:49:55
 */
//
// Created by yuzhi on 5/29/22.
//

#ifndef LEDGERDB_IGRAMMARCOMMAND_H
#define LEDGERDB_IGRAMMARCOMMAND_H

#include <optional>

#include <common/Error.h>
#include <interfaces/IUnique.h>
#include <parser/GrammarType.h>
#include <utility/VaStack.h>

namespace yuzhi::grammar
{

class IGrammarCommand : public IUnique<GrammarType>
{

public:
  using Key = IUnique<GrammarType>::Key;

public:
  virtual std::optional<common::Error> operator()(utility::VaStack &vaStack) = 0;

  virtual GrammarType getUnique() const = 0;

public:
  virtual ~IGrammarCommand() = default;
};
} // namespace yuzhi::grammar

#endif // LEDGERDB_IGRAMMARCOMMAND_H
