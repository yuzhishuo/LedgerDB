#pragma once

#include <string>
#include <map>
#include <set>
#include <type_traits>

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