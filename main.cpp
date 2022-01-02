#include <iostream>
#include <memory>

#include <rocksdb/db.h>
#include <casbin/casbin.h>

#include "AuthorityCertification.h"
#include "User.h"
#include "PersistenceStore.h"


// temp
#include "Meta.h"
// temp
int main(int argc, char **argv)
{
    AuthorityCertification::Instance().Start();
    std::cout << AuthorityCertification::Instance().UserPass("CreateUser", "admin") << std::endl;

    auto &persistenceStore = PersistenceStore::Instance();
    // auto [value, error] = persistenceStore.load("key");

    // if (error)
    // {
    //     std::cout << error.value() << std::endl;
    // }
    // else
    // {
    //     std::cout << value << std::endl;
    // }

    persistenceStore.save("key", "value");
    std::cout << "key is " << persistenceStore.load("key").first << std::endl;

    auto error = persistenceStore.update_key("key", "value2");
    if (error)
    {
        std::cout << error.value() << std::endl;
    }

    std::cout << "key is " << persistenceStore.load("key").first << std::endl;
    return 0;
}
