//
// Created by yuzhi on 5/24/22.
//

#ifndef LEDGERDB_CREATELEDGER_H
#define LEDGERDB_CREATELEDGER_H
#include <spdlog/spdlog.h>

#include "../GammarHandler.h"

namespace yuzhi::grammar {

class CreateLedger : public GammarHandler {

public:
    std::optional<Error> execution(GammarType type ,utility::VaStack& vaStack) override
    {
        auto a0 = vaStack.pop<int>();
        auto a1 = vaStack.pop<const char *>();
        SPDLOG_INFO("type: create_ledger, args is {}, {}", a0, a1);

        return std::nullopt;
    }

    GammarType GetUnique()  const override {
        return GammarType::CREATE_LEDGER;
    }
};
} // ns yuzhi::grammar

#endif //LEDGERDB_CREATELEDGER_H
