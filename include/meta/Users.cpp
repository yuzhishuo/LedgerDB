/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-24 01:40:54
 */
#include "Users.h"
#include "User.h"

namespace yuzhi
{

// TODO: will delete
Users::Users(std::initializer_list<std::pair<std::string, std::shared_ptr<User>>> init)
    : users_{}, usersImpl_(std::weak_ptr<ROCKSDB_NAMESPACE::DB>())
{
}

Users::Users(std::weak_ptr<ROCKSDB_NAMESPACE::DB> db) : users_{}, usersImpl_(db) {}

std::shared_ptr<User> Users::createUser(const std::string &user_name, const std::string &ledger_name, USER_ROLE role)
{
  if (auto itr = users_.find(user_name); itr != users_.end())
  {
    return nullptr;
  }

  auto user = std::make_shared<User>(user_name, ledger_name, role);
  users_.insert(std::make_pair(user_name, user));
  return user;
}

bool Users::removeUser(const std::shared_ptr<User> &user)
{
  auto itr = users_.find(user->name());
  if (itr == users_.end())
  {
    return false;
  }

  users_.erase(itr);
  return true;
}

const std::shared_ptr<User> Users::getUser(const std::string &name) const
{
  if (auto itr = users_.find(name); itr != users_.end())
  {
    return itr->second;
  }
  return nullptr;
}

std::shared_ptr<User> Users::getUser(const std::string &name)
{
  if (auto itr = users_.find(name); itr != users_.end())
  {
    return itr->second;
  }
  return nullptr;
}

} // namespace yuzhi