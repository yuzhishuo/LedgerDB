//
// Created by yuzhi on 5/24/22.
//

#ifndef LEDGERDB_GAMMARHANDLER_H
#define LEDGERDB_GAMMARHANDLER_H


#include <interfaces/IUnique.h>
#include <utility/VaStack.h>
#include <common/Error.h>
#include <optional>

#include "GammarType.h"
namespace yuzhi::grammar {
class GammarHandler: public IUnique<GammarType>{

private:
    virtual std::optional<Error> execution(GammarType type ,utility::VaStack& vaStack) = 0;

public:

    std::optional<Error> operator()(const std::string& type ,utility::VaStack& vaStack);
};
}

#endif //LEDGERDB_GAMMARHANDLER_H
