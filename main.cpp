#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON

#include <iostream>
#include <memory>

#include <rocksdb/db.h>

#include "AuthorityCertification.h"
#include "PersistenceStore.h"
#include "User.h"
#include "Spd.h"

// tmp
#include "IUnique.h"
#include "Users.h"
#include "Ledgers.h"
// tmp
namespace spd = spdlog;

int main(int argc, char **argv)
{
    AuthorityCertification::Instance().Start();
    std::cout << AuthorityCertification::Instance().UserPass("CreateUser", "admin") << std::endl;

    auto &man = Users::getInstance();
    auto t = man.createUser("fff");

    if (t)
    {
        if (auto error = man.store(t); error)
        {
            std::cout << *error << std::endl;
        }
    }

    if (AuthorityCertification::Instance().UserPass("CreateLedger", "fff"))
    {
        auto &man_l = Ledgers::getInstance();
        auto l = man_l.createLedger("f", t->GetUnique());
        man_l.store(l);
    }

    // auto console = spd::stdout_color_mt("console");
    // spd::get("console")->info("Hello, world!");
    // {
    //     auto &persistenceStore = PersistenceStore::getInstance();

    //     persistenceStore.save("key", "value");
    //     std::cout << "key is " << persistenceStore.load("key").first << std::endl;
    //     auto error = persistenceStore.update_key("key", "value2");
    //     if (error)
    //     {
    //         std::cout << error.value() << std::endl;
    //     }

    //     std::cout << "key is " << persistenceStore.load("key").first << std::endl;
    // }
    return 0;
}
