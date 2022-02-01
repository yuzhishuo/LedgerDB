#pragma once

#include <type_traits>

#include <common/Error.h>
#include "IUnique.h"

template <typename T>
class IStorable
{

public:
    static_assert(std::is_base_of_v<IUnique<typename T::Key>, T>,
                  "T must be a subclass of IUnique");

public:
    virtual ~IStorable() {}
    virtual std::optional<Error> store(const std::shared_ptr<T> &object) const = 0;
    virtual std::shared_ptr<T> load(const std::shared_ptr<IUnique<typename T::Key>> &object) = 0;
};