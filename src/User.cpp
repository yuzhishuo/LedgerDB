/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-03-11 01:58:47
 */
#include "meta/User.h"
#include "meta/Users.h"

void User::dispose()
{
    auto is_removed = Users::getInstance().removeUser(shared_from_this());
    assert(is_removed);
}
