#pragma once;
#include <map>
#include <utility>
#include <optional>

#include "IUnique.h"
#include "Error.h"

template <typename T, typename U = typename T::Key>
class SelfDictionary
{

public:
    using Key = U;
    using Value = T;

    static_assert(std::is_base_of_v<T, IUnique<Key>>);
    static_assert(std::is_default_constructible_v<Key>);
    static_assert(std::is_copy_constructible_v<Key>);
    static_assert(std::is_copy_assignable_v<Key>);

public:
    SelfDictionary() = default;
    ~SelfDictionary() = default;

public:
    std::optional<Error> Add(const Value &value)
    {

        auto is_inserted = dictionary_.insert(std::make_pair(value.GetUnique(), value));
        if (!is_inserted.second)
        {
            return Error("Duplicate key");
        }
        return std::nullopt;
    }

    std::optional<Error> Update(const Value &value)
    {
        if (dictionary_.find(value.GetUnique()) == dictionary_.end())
        {
            return Error("Key not found");
        }

        dictionary_.insert(std::make_pair(value.GetUnique(), value));

        return std::nullopt;
    }

    void Remove(const Value &value)
    {
        dictionary_.erase(value.GetUnique());
    }

    void Remove(const Key &key)
    {
        dictionary_.erase(key);
    }

private:
    std::map<Key, Value> dictionary_;
};

using StringUniqueSelfDictionary = SelfDictionary<IUnique<std::string>>;
using IntUniqueSelfDictionary = SelfDictionary<IUnique<int>>;
using Uint64TUniqueSelfDictionary = SelfDictionary<IUnique<uint64_t>>;
using Uint32TUniqueSelfDictionary = SelfDictionary<IUnique<uint32_t>>;
using Uint16TUniqueSelfDictionary = SelfDictionary<IUnique<uint16_t>>;
using Uint8TUniqueSelfDictionary = SelfDictionary<IUnique<uint8_t>>;
using CharUniqueSelfDictionary = SelfDictionary<IUnique<char>>;
using BoolUniqueSelfDictionary = SelfDictionary<IUnique<bool>>;
using FloatUniqueSelfDictionary = SelfDictionary<IUnique<float>>;
using DoubleUniqueSelfDictionary = SelfDictionary<IUnique<double>>;
