//
// Created by yuzhi on 5/19/22.
//

#include <stdio.h> // fpos_t
#include <stdlib.h>
#include <spdlog/spdlog.h>
#include "Parser.h"

extern "C" {
#include "lzhi.tab.h"
int yydebug;
int yyparse();
int yyrestart(FILE*);
}

extern FILE *yyin;

using namespace yuzhi::grammar;

Parser::Parser():fp_{} {}


void Parser::handle(const std::string& type, utility::VaStack& vaStack)
{
    spdlog::info("parser::handle type is {}", type);

    auto& config = Config::Instance();
    auto cli_config = config.get<std::string>(this, "cli");
    GrammarCommandFactory::Instance().get(type)(vaStack);
}


Parser& Parser::load(std::string_view view)
{
    if(fp_)
    {
        fclose(fp_);
        fp_ = nullptr;
    }

    fp_ = (!view.empty())? fopen(view.data(), "r") : nullptr;
    return *this;
}

int Parser::handle(const std::string& sentence)
{
    if(fp_)
    {
        fclose(fp_);
        fp_ = nullptr;
    }

    char file_template_name[] = "template-XXXXXX";
    auto filename = mktemp(file_template_name);

    if(fp_ = fopen(filename, "w+"); fp_)
    {

    }
    spdlog::info("size :{}",sentence.size());
    if( fwrite(sentence.c_str(), sentence.size(), 1,  fp_)!= 1)
    {

    }

    if(fseek(fp_, 0, SEEK_SET))
    {

    }

    return yy();
}

Parser::~Parser()
{
    if(fp_)
        fclose(fp_);
}

int Parser::yy () const
{
    if(fp_)
    {
        yyrestart(fp_);
        return yyparse();
    }
    SPDLOG_WARN("Parser will capture text from stdin");
    return yyparse();
}

const char *Parser::Field() const
{
    return "parser";
}

#include<stdarg.h> // for va_list, va_start, va_end, va_arg
#include <utility/VaStack.h> // for Vastack

extern "C" void emit(char *s, ... )
{
    using yuzhi::utility::VaStack;
    using yuzhi::grammar::Parser;

    extern int yylineno;
    va_list list;
    va_start(list,s);
    VaStack vaStack(list);
    Parser::Instance()->handle(s, vaStack);
}