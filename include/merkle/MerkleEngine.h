#pragma once
#include <string>
#include <memory>

#include <merkle/merklecpp/merklecpp.h>

class Ledger;

namespace yuzhi
{

    const std::string kMerkleRoot = "/tmp/merkle/";

    class MerkleEngine
    {

    public:
        MerkleEngine(std::shared_ptr<Ledger> ledger);
        ~MerkleEngine() = default;

    private:
        std::string tree_store_path_;
        std::shared_ptr<Ledger> ledger_;
        merkle::Tree merkle_;
    };
} // namespace yuzhi