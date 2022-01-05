#pragma once

#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>
#include <mutex>
#include <memory>

#include "IStore.h"

constexpr char const *default_db_path = "./tmp/store/rockdb/db0";
constexpr char const *default_default_family = "default";
class PersistenceStore : public IStorage
{
public:
    PersistenceStore(std::string family = default_default_family, const std::string &path = default_db_path)
        : db_path_(path),
          db_(nullptr),
          options_(rocksdb::Options()),
          family_(family),
          cf_(nullptr)
    {
        _RockdbInit();
    }

private:
    void _RockdbInit() noexcept
    {
#ifdef DEBUG
        auto ds_status = rocksdb::DestroyDB(db_path_, rocksdb::Options());
        if (!ds_status.ok())
        {
            std::cout << "DestroyDB Error: " << ds_status.ToString() << std::endl;
        }
#endif // __DEBUG

        options_.create_if_missing = true;

        rocksdb::Status db_status = rocksdb::DB::Open(options_, db_path_, &db_);
        if (!db_status.ok())
        {
            std::cout << "PersistenceStore: " << db_status.ToString() << std::endl;
        }

        if (family_.empty() || family_ == default_default_family)
        {
            cf_ = db_->DefaultColumnFamily();
        }
        else
        {
            rocksdb::ColumnFamilyOptions options;
            auto f_status = db_->CreateColumnFamily(options, family_, &cf_);
            if (!f_status.ok())
            {
                std::cout << "PersistenceStore: " << f_status.ToString() << std::endl;
            }
        }
    }

public:
    virtual ~PersistenceStore()
    {
        delete db_;
    }

    virtual std::optional<Error> save(const std::string &key, const std::string &value) override
    {

        rocksdb::Status status = db_->Put(rocksdb::WriteOptions(), cf_, key, value);
        if (!status.ok())
        {
            return std::make_optional<Error>(status.ToString());
        }
        return std::nullopt;
    }

    virtual std::pair<std::string, std::optional<Error>> load(const std::string &key) override
    {
        std::string value;
        rocksdb::Status status = db_->Get(rocksdb::ReadOptions(), cf_, key, &value);
        if (!status.ok())
        {
            return std::make_pair("", std::make_optional<Error>(status.ToString()));
        }
        return std::make_pair(value, std::nullopt);
    }

    virtual std::optional<Error> delete_key(const std::string &key) override
    {
        rocksdb::Status status = db_->Delete(rocksdb::WriteOptions(), cf_, key);
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
        auto read_status = db_->Get(rocksdb::ReadOptions(), cf_, key, &old_value);
        if (!read_status.ok())
        {
            return std::make_optional<Error>(read_status.ToString());
        }

        auto write_status = db_->Put(rocksdb::WriteOptions(), cf_, key, value);

        if (!write_status.ok())
        {
            return std::make_optional<Error>(write_status.ToString());
        }

        return std::nullopt;
    }

    static PersistenceStore &getInstance()
    {
        static PersistenceStore store("", "/tmp/store/ledgerdb");
        return store;
    }

private:
    const std::string db_path_;
    const std::string family_;
    rocksdb::Options options_;
    rocksdb::DB *db_;
    rocksdb::ColumnFamilyHandle *cf_;
};