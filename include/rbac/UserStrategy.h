/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-03-11 02:01:46
 */
#pragma once

#include "rbac/AuthorityCertification.h"
#include "Strategy.h"
#include "meta/Role.h"
#include "utility.h"

#include <initializer_list>
#define DEFINE_USER_STRATEGY(name, args...)                                      \
    class name##Strategy : public IStrategy                                      \
    {                                                                            \
    public:                                                                      \
        name##Strategy()                                                         \
            : IStrategy(#name, std::vector<std::any>{args}) {}                   \
                                                                                 \
        virtual bool eq(const std::any &le, const std::any &ri) const override   \
        {                                                                        \
            return std::any_cast<USER_ROLE>(le) == std::any_cast<USER_ROLE>(ri); \
        }                                                                        \
    }

DEFINE_USER_STRATEGY(CreateUser, std::any(USER_ROLE::DBA));
DEFINE_USER_STRATEGY(DeleteUser, USER_ROLE::DBA);
DEFINE_USER_STRATEGY(CreateLedger, USER_ROLE::DBA, USER_ROLE::COMMON);

#define UserAuthorityCertificationHelper      \
    AuthorityCertificationHelper(CreateUser); \
    AuthorityCertificationHelper(DeleteUser); \
    AuthorityCertificationHelper(CreateLedger)
