#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>
#include <mutex>
#include <memory>

#include "IStore.h"

constexpr char const *default_db_path = "./tmp/store/rockdb/db0";

class PersistenceStore : public IStorage
{
public:
    PersistenceStore(const std::string &path = default_db_path)
        : db_path_(path),
          db_(nullptr),
          options_(rocksdb::Options())
    {
#ifdef DEBUG
        auto ds_status = rocksdb::DestroyDB(path, rocksdb::Options());
        if (!ds_status.ok())
        {
            std::cout << "DestroyDB Error: " << ds_status.ToString() << std::endl;
        }
#endif // __DEBUG

        options_.create_if_missing = true;

        rocksdb::Status status = rocksdb::DB::Open(options_, path, &db_);
        if (!status.ok())
        {
            std::cout << status.ToString() << std::endl;
        }
    }

    virtual ~PersistenceStore()
    {
        delete db_;
    }

    virtual std::optional<Error> save(const std::string &key, const std::string &value) override
    {

        rocksdb::Status status = db_->Put(rocksdb::WriteOptions(), key, value);
        if (!status.ok())
        {
            return std::make_optional<Error>(status.ToString());
        }
        return std::nullopt;
    }

    virtual std::pair<std::string, std::optional<Error>> load(const std::string &key) override
    {
        std::string value;
        rocksdb::Status status = db_->Get(rocksdb::ReadOptions(), key, &value);
        if (!status.ok())
        {
            return std::make_pair("", std::make_optional<Error>(status.ToString()));
        }
        return std::make_pair(value, std::nullopt);
    }

    virtual std::optional<Error> delete_key(const std::string &key) override
    {
        rocksdb::Status status = db_->Delete(rocksdb::WriteOptions(), key);
        if (!status.ok())
        {
            return std::make_optional<Error>(status.ToString());
        }
        return std::nullopt;
    }

    virtual std::optional<Error> update_key(const std::string &key, const std::string &value) override
    {
        // using namespace rocksdb;
        // TODO: use optimistic transaction
        std::string old_value;
        auto read_status = db_->Get(rocksdb::ReadOptions(), key, &old_value);
        if (!read_status.ok())
        {
            return std::make_optional<Error>(read_status.ToString());
        }

        auto write_status = db_->Put(rocksdb::WriteOptions(), key, value);

        if (!write_status.ok())
        {
            return std::make_optional<Error>(write_status.ToString());
        }

        return std::nullopt;
    }

    static PersistenceStore &Instance()
    {
        static PersistenceStore store("/tmp/store/ledgerdb");
        return store;
    }

private:
    const std::string db_path_;
    rocksdb::Options options_;
    rocksdb::DB *db_;
};