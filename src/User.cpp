#include "User.h"

std::shared_ptr<User> User::admin = std::make_shared<User>("admin", USER_ROLE::DBA);