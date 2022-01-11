#pragma once

#include <rocksdb/db.h>
#include <rocksdb/utilities/transaction.h>
#include <rocksdb/utilities/optimistic_transaction_db.h>

#include <mutex>
#include <memory>
#include <string>
#include <vector>

#include "IStore.h"

constexpr char const *default_db_path = "/tmp/store/rockdb/";
constexpr char const *db_name = "db";
constexpr char const *default_default_family = "default";

class PersistenceStore : public IStorage
{
public:
    PersistenceStore(std::string family = default_default_family, const std::string &path = db_name)
        : db_path_(std::string(default_db_path) + path),
          db_(nullptr),
          options_(rocksdb::Options()),
          family_(family),
          handles{}
    {
        _RockdbInit();
    }

private:
    void _RockdbInit() noexcept
    {
        // #ifdef DEBUG
        //         if (auto ds_status = rocksdb::DestroyDB(db_path_, rocksdb::Options()); !ds_status.ok())
        //         {
        //             std::cout << "DestroyDB Error: " << ds_status.ToString() << std::endl;
        //         }
        // #endif // __DEBUG

        options_.create_if_missing = true;

        auto column_families = std::vector{
            rocksdb::ColumnFamilyDescriptor(rocksdb::kDefaultColumnFamilyName, rocksdb::ColumnFamilyOptions()),
            rocksdb::ColumnFamilyDescriptor(family_, rocksdb::ColumnFamilyOptions())};

        do
        {
            if (rocksdb::Status db_status = rocksdb::DB::Open(options_, db_path_, column_families, &handles, &db_); !db_status.ok())
            {
                std::cout << "PersistenceStore:: Open" << db_status.ToString() << std::endl;
                column_families.pop_back();
            }
            else
            {
                break;
            }
        } while (true);

        if (family_.empty() || family_ == default_default_family /*need test*/)
        {
            handles.push_back(nullptr);
            std::swap(handles[0], handles[1]);
        }
        else
        {
            assert(handles.size() >= 1);
            if (handles.size() < 2)
            {
                rocksdb::ColumnFamilyOptions options;
                rocksdb::ColumnFamilyHandle *cf = nullptr;
                if (auto f_status = db_->CreateColumnFamily(options, family_, &cf); handles.size() == 1 && !f_status.ok())
                {
                    std::cout << "PersistenceStore: CreateColumnFamily" << f_status.ToString() << std::endl;
                }
                assert(cf);
                handles.push_back(cf);
            }
        }
    }

public:
    virtual ~PersistenceStore()
    {
        for (auto handle : handles)
        {
            if (handle)
            {
                auto s = db_->DestroyColumnFamilyHandle(handle);
                assert(s.ok());
            }
        }
        delete db_;
    }

    virtual std::optional<Error> save(const std::string &key, const std::string &value) override
    {
        {
            std::string old_value;
            if (auto read_status = db_->KeyMayExist(rocksdb::ReadOptions(), handles[1], key, &old_value); read_status)
            {
                return Error{"Key already exists"};
            }
        }

        if (rocksdb::Status status = db_->Put(rocksdb::WriteOptions(), handles[1], key, value); !status.ok())
        {
            return std::make_optional<Error>(status.ToString());
        }
        return std::nullopt;
    }

    virtual std::pair<std::string, std::optional<Error>> load(const std::string &key) override
    {
        std::string value;

        if (rocksdb::Status status = db_->Get(rocksdb::ReadOptions(), handles[1], key, &value); !status.ok())
        {
            return std::make_pair("", std::make_optional<Error>(status.ToString()));
        }
        return std::make_pair(value, std::nullopt);
    }

    virtual std::optional<Error> delete_key(const std::string &key) override
    {

        if (rocksdb::Status status = db_->Delete(rocksdb::WriteOptions(), handles[1], key); !status.ok())
        {
            return std::make_optional<Error>(status.ToString());
        }
        return std::nullopt;
    }

    virtual std::optional<Error> update_key(const std::string &key, const std::string &value) override
    {
        // using namespace rocksdb;
        // TODO: use optimistic transaction
        {
            std::string old_value;
            if (auto read_status = db_->Get(rocksdb::ReadOptions(), handles[1], key, &old_value); !read_status.ok())
            {
                return std::make_optional<Error>(read_status.ToString());
            }
        }

        if (auto write_status = db_->Put(rocksdb::WriteOptions(), handles[1], key, value); !write_status.ok())
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
    std::vector<rocksdb::ColumnFamilyHandle *> handles;
};