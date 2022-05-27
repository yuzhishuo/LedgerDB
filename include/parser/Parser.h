//
// Created by yuzhi on 5/19/22.
//

#ifndef LEDGERDB_PARSER_H
#define LEDGERDB_PARSER_H

#include <stdio.h>
#include <stdlib.h> // for malloc, exit
#include <string.h>
#include <memory> // for std::shared_ptr

#include <utility/VaStack.h>
#include <config/IConfigurable.hpp> // for interface IConfigurable

#include "exector/CreateLedger.h"

namespace yuzhi::grammar {

    class Parser : public IConfigurable {
    public:
        static std::shared_ptr<Parser> Instance()
        {
            static std::shared_ptr<Parser> parser = std::make_shared<Parser>();
            return parser;
        }

    public:
        void handle(const std::string& type, utility::VaStack& vaStack)
        {
            spdlog::info("parser::handle type is {}", type);
            auto createLedger = new CreateLedger;
            createLedger->operator()(type, vaStack);
        }
    public:
        Parser();
        ~Parser();
    public:
        Parser& load(std::string_view view);
        int handle(const std::string& sentence);
        int yy () const;
    public:
        virtual const char *Field() const {
            return "parser";
        }
    private:
        FILE* fp_;
    };

} // namespace yuzhi::parser

#endif // LEDGERDB_PARSER_H
