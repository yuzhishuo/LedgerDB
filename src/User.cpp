#include "User.h"
#include "Users.h"

void User::dispose()
{
    auto is_removed = Users::getInstance().removeUser(shared_from_this());
    assert(is_removed);
}
