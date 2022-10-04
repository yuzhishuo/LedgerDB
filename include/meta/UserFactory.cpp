/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-17 11:18:24
 */
#include "UserFactory.h"
#include "User.h"
#include <exception>

namespace yuzhi
{


UserFactory::UserFactory(std::weak_ptr<ROCKSDB_NAMESPACE::DB> db) : users_{}, usersImpl_(db) {}

std::shared_ptr<User> UserFactory::createUser(const std::string &user_name, const std::string &ledger_name,
                                              USER_ROLE role)
{
  if (auto itr = users_.find(user_name); itr != users_.end())
  {
    return nullptr;
  }

  auto user = std::make_shared<User>(user_name, ledger_name, role);
  users_.try_emplace(user_name, user);
  return user;
}

bool UserFactory::removeUser(const std::shared_ptr<User> &user)
{
  auto itr = users_.find(user->name());
  if (itr == users_.end())
  {
    return false;
  }

  users_.erase(itr);
  return true;
}

const std::shared_ptr<User> UserFactory::getUser(const std::string &name) const
{
  if (auto itr = users_.find(name); itr != users_.end())
  {
    return itr->second;
  }
  return nullptr;
}

std::shared_ptr<User> UserFactory::getUser(const std::string &name)
{
  if (auto itr = users_.find(name); itr != users_.end())
  {
    return itr->second;
  }
  return nullptr;
}

} // namespace yuzhi