#include <rocksdb/db.h>
#include <mutex>
#include <memory>

class Store
{
public:
    Store(const std::string &path)
    {
        rocksdb::Options options;
        options.create_if_missing = true;

        rocksdb::Status status = rocksdb::DB::Open(options, path, &db_);
        if (!status.ok())
        {
            std::cout << status.ToString() << std::endl;
        }
    }

    ~Store()
    {
        db_->Close();
    }

    void Put(const std::string &key, const std::string &value)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        db_->Put(rocksdb::WriteOptions(), key, value);
    }
    
    std::string Get(const std::string &key)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string value;
        db_->Get(rocksdb::ReadOptions(), key, &value);
        return value;
    }

    void Delete(const std::string &key)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        db_->Delete(rocksdb::WriteOptions(), key);
    }

    static Store &Instance()
    {
        static Store store("/tmp/store");
        return store;
    }

private:
    std::mutex mutex_;
    rocksdb::DB *db_;
};