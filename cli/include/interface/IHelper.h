//
// Created by yuzhi on 6/21/22.
//

#ifndef LEDGERDB_IHELPER_H
#define LEDGERDB_IHELPER_H

#include <string>
#include <utility>

namespace yuzhi::cli {

class IHelper {

  public:
    virtual std::pair<std::string, std::string> explain() const = 0;;
};

} // namespace yuzi::cli

#endif//LEDGERDB_IHELPER_H
