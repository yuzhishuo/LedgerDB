#pragma onece

#include "Strategy.h"
#include "utility.h"
#include "Role.h"

#include <initializer_list>

#define DEFINE_LEDGER_STRATEGY(name, args...)                                        \
    class name##Strategy : public IStrategy                                          \
    {                                                                                \
    public:                                                                          \
        name##Strategy() : IStrategy(#name, std::vector<std::any>{{args}}) {}        \
        virtual bool eq(const std::any &le, const std::any &ri) const override       \
        {                                                                            \
            return std::any_cast<LEDGER_ROLE>(le) == std::any_cast<LEDGER_ROLE>(ri); \
        }                                                                            \
    }

DEFINE_LEDGER_STRATEGY(Read, std::any(LEDGER_ROLE::OWNER), LEDGER_ROLE::REGULATOR, LEDGER_ROLE::REGULATOR, LEDGER_ROLE::COMMON);
DEFINE_LEDGER_STRATEGY(Write, LEDGER_ROLE::OWNER);
DEFINE_LEDGER_STRATEGY(Delete, LEDGER_ROLE::OWNER);
DEFINE_LEDGER_STRATEGY(Verify, LEDGER_ROLE::OWNER, LEDGER_ROLE::REGULATOR, LEDGER_ROLE::COMMON, LEDGER_ROLE::READONLY);
DEFINE_LEDGER_STRATEGY(Grant, LEDGER_ROLE::OWNER);
DEFINE_LEDGER_STRATEGY(Purge, LEDGER_ROLE::OWNER);

#define LedgerAuthorityCertificationHelper \
    AuthorityCertificationHelper(Read);    \
    AuthorityCertificationHelper(Write);   \
    AuthorityCertificationHelper(Delete);  \
    AuthorityCertificationHelper(Verify);  \
    AuthorityCertificationHelper(Grant);   \
    AuthorityCertificationHelper(Purge)
