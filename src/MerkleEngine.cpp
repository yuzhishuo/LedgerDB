#include <fstream>
#include <filesystem>

#include <merkle/MerkleEngine.h>
#include <Ledger.h>

using namespace yuzhi;
using namespace std;

namespace fs = std::filesystem;

MerkleEngine::MerkleEngine(std::shared_ptr<Ledger> ledger)
    : tree_store_path_(kMerkleRoot),
      ledger_(ledger)
{
    if (!fs::exists(tree_store_path_))
    {
        fs::create_directories(tree_store_path_);
    }

    const std::string tree_file_path = tree_store_path_ + "Merkle" + to_string(ledger_->id());

    if (fs::exists(tree_file_path))
    {

        std::ifstream ifs(tree_file_path);
        // if (!ifs.is_open())
        // {
        //     throw std::runtime_error("open tree file failed");
        // }
        std::string tree_str;
        ifs >> tree_str;
        merkle_->deserialise(std::vector<uint8_t>(tree_str.begin(), tree_str.end()));
    }
    else
    {
        std::ofstream ofs(tree_file_path, std::ios::binary | std::ios::ate);

        std::vector<uint8_t> tree_data = {0};
        ofs << std::string(tree_data.begin(), tree_data.end());
    }

    // std::cout << "merkle engine init" << std::endl;

    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, tree_store_path_, &db);
    if (!status.ok())
    {
        throw std::runtime_error("open db failed");
    }
}
