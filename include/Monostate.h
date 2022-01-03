#pragma once

#include <optional>

#include "IUnique.h"
#include "Error.h"

// duck model
template <typename T, typename U>
class Monostate
{
public:
    Monostate() = default;
    ~Monostate() = default;

public:
    static Monostate<T> &getInstance()
    {
        static Monostate<T> instance;
        return instance;
    }

public:
    T load(const IUnique<U> &unique)
    {


        // auto it = dictionary_.find(unique.GetUnique());
        // if (it == dictionary_.end())
        // {
        //     return T();
        // }
        // return it->second;
    }

    std::optional<Error> save(const T &value)
    {
        // auto is_inserted = dictionary_.insert(std::make_pair(value.GetUnique(), value));
        // if (!is_inserted.second)
        // {
        //     return Error("Duplicate key");
        // }
        return std::nullopt;
    }
};

// Monostate<T> is a type-level object that represents the empty state of a type.
// It is a singleton, and can be accessed via the getInstance() method.
//
// The following code snippet demonstrates how to use Monostate<T> to implement
// a type-level empty state.
//  template <typename T>
//  class Monostate
//  {
//  public:
//      static Monostate<T> &getInstance()
//      {
//          static Monostate<T> instance;
//          return instance;
//      }
//  };