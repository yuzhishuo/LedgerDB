#include <iostream>
#include <casbin/casbin.h>
#include <memory>




int main(int, char **)
{
    {

        auto m = std::make_shared<casbin::Model>("./rbac/user/rbac_model.conf");

        auto a = std::make_shared<casbin::FileAdapter>("./rbac/user/rbac_policy.csv");

        auto e = casbin::Enforcer(m, std::dynamic_pointer_cast<casbin::Adapter>(a));

        auto sub = "admin";
        auto obj = "CREATE_USER";
        auto status_r = e.RemovePolicy({sub, obj});


        auto ok = e.Enforce({sub, obj});

        if (ok)
        {
            std::cout << "auth sucessor" << std::endl;
        }

        auto roles = e.GetRolesForUser(sub);

        for (auto role : roles)
        {
            std::cout << "sub: " << sub << ",role is " << role << std::endl;
        }
    }

    {

        auto m = std::make_shared<casbin::Model>("./rbac/ledger/rbac_model.conf");

        auto a = std::make_shared<casbin::FileAdapter>("./rbac/ledger/rbac_policy.csv");

        auto e = casbin::Enforcer(m, std::dynamic_pointer_cast<casbin::Adapter>(a));

        auto sub = "admin";
        auto obj = "Read";
        auto act = "luluyuzhi";

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
    }
    return 0;
}
