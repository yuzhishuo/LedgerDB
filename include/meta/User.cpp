/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-03-11 14:13:06
 */
#include "User.h"
#include "Users.h"

void User::dispose()
{
    auto is_removed = Users::getInstance().removeUser(shared_from_this());
    assert(is_removed);
}
