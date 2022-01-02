#pragma onece

#include "Role.h"
#include "Ledger.h"

template<>
using LedgerRole = Role<LEDGER_ROLE, Ledger>;