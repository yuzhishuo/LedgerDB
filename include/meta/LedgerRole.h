/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 06:08:57
 */
#pragma onece

#include "Ledger.h"
#include "Role.h"

template <> using LedgerRole = Role<LEDGER_ROLE, Ledger>;