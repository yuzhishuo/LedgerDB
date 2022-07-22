/*
 * @Author: Leo
 * @Date: 2022-02-01 21:47:19
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:21:16
 */
#pragma once

#include <string>

#include <interfaces/IStorable.h>

namespace yuzhi::store
{
class IStorage;

template <typename T> class StoreCreator : public IStorable<T>
{
public:
  StoreCreator(const std::string &prefix = "",
               const std::string family = "default",
               const std::string &suffix = "")
      : prefix_(prefix), suffix_(suffix), family_(family), IStorable<T>()
  {
  }
  virtual ~StoreCreator() = default;
  virtual IStorage *create() const = 0;
  virtual std::optional<common::Error>
  store(const std::shared_ptr<T> &object) const
  {

    if (auto storage = create(); storage)
    {
      if (auto [raw, err] = object->serialize(); !err)
      {
        auto key = prefix_ + "_" + object->GetUnique() + "_" + suffix_;
        return storage->save(key, raw);
      }
      else
      {
        return err;
      }
    }

    return common::Error{"Failed to create storage"};
  }

  virtual std::shared_ptr<T>
  load(const std::shared_ptr<IUnique<typename T::Key>> &object) const
  {
    if (auto storage = create(); storage)
    {
      auto key = prefix_ + "_" + object->GetUnique() + "_" + suffix_;

      if (auto [store_raw, err] = storage->load(key); !err)
      {
        // if (auto [res, e]= object->deserialize(store_raw); e)
        // {
        //     return nullptr;
        // }
        // else
        // {
        //     return T{*res};
        // }
      }
      return nullptr;
    }

    return nullptr;
  }

private:
  std::string prefix_;
  std::string suffix_;
  std::string family_;
};

} // namespace yuzhi::store
