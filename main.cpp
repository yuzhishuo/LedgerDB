#include <iostream>
#include <casbin/casbin.h>
#include <memory>

// casbin::ReturnType myMatcher(casbin::Scope scope)
// {
//     std::string key1 = casbin::GetString(scope, 0);
//     std::string key2 = casbin::GetString(scope, 1);
//     casbin::PushBooleanValue(scope, false);
//     return -1;
// }

#include "AuthorityCertification.h"
#include "User.h"

int main(int argc, char **argv)
{
    AuthorityCertification::Instance().Start();
    std::cout << AuthorityCertification::Instance().UserPass("CreateUser", "admin") << std::endl;
    // auto m = std::make_shared<casbin::Model>("./rbac/rbac_model.conf");

    // auto a = std::make_shared<casbin::FileAdapter>("./rbac/rbac_policy.csv");
    // try
    // {
    //     auto e = casbin::Enforcer((m), std::dynamic_pointer_cast<casbin::Adapter>(a));

    //     auto sub = "jiansu";
    //     auto obj = "Write";
    //     auto obj1 = "luluyuzhi";
    //     auto act = "allow";
    //     e.EnableLog(true);
    //     auto ok = e.Enforce({sub, obj, obj1, act});

    //     // e.AddFunction("myMatcher", myMatcher, 2);
    //     if (ok)
    //     {
    //         std::cout << "auth sucessor" << std::endl;
    //     }

    //     auto roles = e.GetUsersForRole("REGULATOR");

    //     for (auto &role : roles)
    //     {
    //         for (auto &r : role)
    //         {
    //             std::cout << "sub: " << sub << ",role is " << r << std::endl;
    //         }
    //     }
    // }
    // catch (std::exception &e)
    // {
    //     std::cout << e.what() << std::endl;
    // }

    return 0;
}
