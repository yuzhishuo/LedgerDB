/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:16:34
 */
#include "User.h"
#include "Users.h"

using namespace yuzhi;

void User::dispose()
{
  auto is_removed = Users::getInstance().removeUser(shared_from_this());
  assert(is_removed);
}
