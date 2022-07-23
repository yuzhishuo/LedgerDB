/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 15:17:24
 */
#include "Users.h"
#include "User.h"

namespace yuzhi
{

Users::Users(std::initializer_list<std::pair<std::string, std::shared_ptr<User>>> init)
    : users_{}, store_creator_{dynamic_cast<IStorable<User> *>(new UserStoreCreator{"User"})},
      usersImpl_(std::weak_ptr<ROCKSDB_NAMESPACE::DB>())
{
  for (auto &[name, user] : init)
  {
    if (auto erro = store(user); !erro)
    {
      users_.insert(std::make_pair(name, user));
    }
    else
    {
    }
  }
}

Users::Users(std::weak_ptr<ROCKSDB_NAMESPACE::DB> db)
    : users_{}, store_creator_{dynamic_cast<IStorable<User> *>(new UserStoreCreator{"User"})}, usersImpl_(db)
{
}

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
  // assert(itr->second.unique());
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