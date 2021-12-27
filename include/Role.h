#pragma once

#include <string>
#include <map>
#include <set>
#include <type_traits>

#include "User.h"
#include "Ledger.h"


template <typename T, typename U>
class Role
{
public:
    using RoleType = T;
    using OnwerType = U;
    using OwnerTypeSmpartPtr = std::weak_ptr<OnwerType>;

    static_assert(std::is_enum_v<RoleType>, "RoleType(T) must be an enum");

    Role(std::shared_ptr<Role::OnwerType> onwer, RoleType role) : role_{role}, owner_{onwer} {}
    Role(RoleType role) : role_{role} {}

    void setOwner(std::shared_ptr<U> onwer)
    {
        owner_ = onwer;
    }

    RoleType role() const
    {
        return role_;
    }

private:
    RoleType role_;
    OwnerTypeSmpartPtr owner_;
};

// class UserRole : public Role<USER_ROLE, User>
// {
// private:
//     using BaseType = Role<USER_ROLE, User>;

// public:
//     using BaseType::Role;
// };

// class LedgerRole : public Role<LEDGER_ROLE, Role>
// {

// private:
//     using BaseType = Role<USER_ROLE, User>;
// };

// // admin -> createUser
// std::map<USER_ROLE, std::set<std::string>> user_role_map = {
//     {USER_ROLE::DBA, {"admin"}},
//     {USER_ROLE::REGULATOR, {"regulator"}},
//     {USER_ROLE::COMMON, {"common"}},
// };

// bool newCommonUser(const std::string &user_name)
// {
//     if (user_role_map[USER_ROLE::COMMON].find(user_name) != user_role_map[USER_ROLE::COMMON].end())
//     {
//         return false;
//     }
//     user_role_map[USER_ROLE::COMMON].insert(user_name);
//     return true;
// }

// bool newRegulatorUser(const std::string &user_name)
// {
//     if (user_role_map[USER_ROLE::REGULATOR].find(user_name) != user_role_map[USER_ROLE::REGULATOR].end())
//     {
//         return false;
//     }
//     user_role_map[USER_ROLE::REGULATOR].insert(user_name);
//     return true;
// }

// bool CreatUser(const std::string &user_name, USER_ROLE role)
// {
//     if (role == USER_ROLE::REGULATOR)
//     {
//         return newRegulatorUser(user_name);
//     }
//     else if (role == USER_ROLE::COMMON)
//     {
//         return newCommonUser(user_name);
//     }
//     return false;
// }

// bool DeleteUser(const std::string &user_name)
// {
//     for (auto &role : user_role_map)
//     {
//         if (role.second.find(user_name) != role.second.end())
//         {
//             role.second.erase(user_name);
//             return true;
//         }
//     }
//     return false;
// }
