#include <iostream>
#include <casbin/casbin.h>
#include <memory>

int main(int, char **)
{

    // auto e = casbin::Enforcer("rbac/rbac_model.conf", "rbac/rbac_policy.csv");

    auto m = std::make_shared<casbin::Model>("rbac/rbac_model.conf");

    auto a = std::make_shared<casbin::FileAdapter>("rbac/rbac_policy.csv");

    auto e = casbin::Enforcer((m), std::dynamic_pointer_cast<casbin::Adapter>(a));

    auto sub = "alice";
    auto obj = "data1";
    auto act = "read";

    auto ok = e.Enforce({sub, obj, act});

    if (ok)
    {
        std::cout << "auth sucessor" << std::endl;
    }

    auto roles = e.GetRolesForUser(sub);

    for (auto role : roles)
    {
        std::cout << "sub: " << sub << ",role is " << role << std::endl;
    }

    return 0;
}
