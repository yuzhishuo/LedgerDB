#include <iostream>
#include <casbin/casbin.h>
#include <memory>


#include "AuthorityCertification.h"
#include "User.h"

int main(int argc, char **argv)
{
    AuthorityCertification::Instance().Start();
    std::cout << AuthorityCertification::Instance().UserPass("CreateUser", "admin") << std::endl;
    return 0;
}
