//
// Created by yuzhi on 6/21/22.
//

#ifndef LEDGERDB_ITRANSACTION_H
#define LEDGERDB_ITRANSACTION_H

#include <store/IStore.h>

namespace yuzhi::store
{
class ITransaction : public IStorage
{
};

}; // namespace yuzhi::store

#endif // LEDGERDB_ITRANSACTION_H
