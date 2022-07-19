/*
 * @Author: Leo
 * @Date: 2022-07-17 14:09:42
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-19 10:27:42
 */
#pragma once
#include<optional>
#include<string>

#include <ledger_engine.pb.h>
namespace yuzhi {
class LedgersImpl
{
public:
    LedgersImpl() = default;
    ~LedgersImpl() = default;

public:

    /*
        create ledger with name and owner
        create user 
    */
    // createLedger  
    std::optional<int> createLedger(ledger_engine::Ledger& mono_ledger)
    {
        return std::nullopt;
    }
    /*
        delete user
        delete ledger
    */
    // deleteLedger
    std::optional<int> deleteLedger(const std::string& ledger_name)
    {
        return std::nullopt;
    }
};
} // namespace yuzhi