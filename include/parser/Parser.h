/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:50:00
 */
//
// Created by yuzhi on 5/19/22.
//

#ifndef LEDGERDB_PARSER_H
#define LEDGERDB_PARSER_H

#include <memory> // for std::shared_ptr
#include <parser/GrammarCommandFactory.h>
#include <stdio.h>
#include <stdlib.h> // for malloc, exit
#include <string.h> // for std::string

#include <config/Config.h>
#include <config/IConfigurable.hpp> // for interface IConfigurable
#include <utility/VaStack.h>

namespace yuzhi::grammar
{

class Parser : public IConfigurable
{
public:
  static std::shared_ptr<Parser> Instance()
  {
    static std::shared_ptr<Parser> parser = std::make_shared<Parser>();
    return parser;
  }

public:
  Parser();
  ~Parser();

public:
  void handle(const std::string &type, utility::VaStack &vaStack);

public:
  const char *Field() const override;
  Parser &load(std::string_view view);
  int handle(const std::string &sentence);
  int yy() const;

private:
  FILE *fp_;
};

} // namespace yuzhi::grammar

#endif // LEDGERDB_PARSER_H
