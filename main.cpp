#include <iostream>
#include <memory>

#include <rocksdb/db.h>
#include <casbin/casbin.h>

#include "AuthorityCertification.h"
#include "User.h"
#include "utility/Singleton.h"

int main(int argc, char **argv)
{
    AuthorityCertification::Instance().Start();
    std::cout << AuthorityCertification::Instance().UserPass("CreateUser", "admin") << std::endl;
    
    
    Store::Instance().Put("key", "value");
    std::cout << Store::Instance().Get("key") << std::endl;
    return 0;
}
