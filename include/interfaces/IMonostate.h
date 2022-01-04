#pragma once

#include <optional>

#include "IUnique.h"
#include "Error.h"

template <typename T>
class IMonostate
{
public:
    virtual ~IMonostate() = default;
    virtual std::pair<const std::string &, std::optional<Error>> serialize() const;
    virtual std::pair<std::shared_ptr<T>, std::optional<Error>> deserialize(const std::string &serialized);
};

// duck model
template <typename T>
class Monostate : public IMonostate<T>
{
public:
    Monostate() = default;
    ~Monostate() = default;
    Monostate(const T &t) : value_(t) {}
    Monostate(T &&t) : value_(std::move(t)) {}

private:
    Monostate(const Monostate &other) = delete;
    Monostate(Monostate &&other) = delete;
    Monostate &operator=(const Monostate &other) = delete;

public:
    virtual std::pair<const std::string &, std::optional<Error>> serialize() const override
    {
        if (auto serialized = std::string(); value_.SerializeToString(&serialized))
        {
            return {serialized, std::nullopt};
        }

        return std::make_pair(std::string{}, Error{"Failed to serialize Monostate"});
    }

    virtual std::pair<std::shared_ptr<T>, std::optional<Error>> deserialize(const std::string &serialized) override
    {
        if (value_.ParseFromString(serialized))
        {
            return {std::make_shared<T>(value_), std::nullopt};
        }
        else
        {
            return {nullptr, Error{"Failed to deserialize Monostate"}};
        }
    }

public:
    T *operator->()
    {
        return &value_;
    }
    const T *operator->() const
    {
        return &value_;
    }
    T &operator*()
    {
        return value_;
    }
    const T &operator*() const
    {
        return value_;
    }

private:
    T value_;
};