//
// Created by yuzhi on 5/29/22.
//

#ifndef LEDGERDB_IGRAMMARCOMMAND_H
#define LEDGERDB_IGRAMMARCOMMAND_H

#include <optional>

#include <common/Error.h>
#include <parser/GrammarType.h>
#include <interfaces/IUnique.h>
#include <utility/VaStack.h>

namespace yuzhi::grammar {

    class IGrammarCommand : public IUnique<GrammarType> {

    public:
        using Key = IUnique<GrammarType>::Key;
    public:
        virtual std::optional <Error> operator()(utility::VaStack& vaStack) = 0;

        virtual GrammarType GetUnique() const = 0;

    public:
        virtual ~IGrammarCommand() = default;
    };
} // yuzhi::grammar

#endif //LEDGERDB_IGRAMMARCOMMAND_H