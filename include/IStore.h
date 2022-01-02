#pragma once

#include <string>
#include <optional>
#include "Error.h"

class IStorage
{

public:
    virtual ~IStorage() = default;
    virtual std::optional<Error> save(const std::string &key, const std::string &value) = 0;
    virtual std::pair<std::string, std::optional<Error>> load(const std::string &key) = 0;
    virtual std::optional<Error> delete_key(const std::string &key) = 0;
    virtual std::optional<Error> update_key(const std::string &key, const std::string &value) = 0;
};