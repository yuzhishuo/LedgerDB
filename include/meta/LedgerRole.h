/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-17 12:22:31
 */
#pragma onece

#include <meta/Ledger.h>
#include "Role.h"

template <> using LedgerRole = Role<LEDGER_ROLE, Ledger>;