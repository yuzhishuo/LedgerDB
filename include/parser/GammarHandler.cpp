//
// Created by yuzhi on 5/24/22.
//

#include "GammarHandler.h"
#include <utility/Enum.h>
using namespace yuzhi::grammar;

std::optional<Error> GammarHandler::operator()(const std::string& type ,utility::VaStack& vaStack)
{
    if(auto type_ = magic_enum::enum_cast<GammarType>(type); type_.has_value())
    {
        return execution(type_.value(), vaStack);
    }

    return Error::UndefineGammarError();
}