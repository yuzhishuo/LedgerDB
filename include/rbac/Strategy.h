#pragma once

#include <string>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

#include <any>
#include <utility>

class IStrategy
{

public:
    IStrategy(const std::string &name, std::vector<std::any> &&r)
        : name_(name), roles_(std::move(r))
    {
    }
    virtual ~IStrategy() {}

    IStrategy & operator=(const IStrategy &) = delete;

public:
    bool Pass(std::any role) const
    {
        for (const auto &r : roles_)
        {
            if (!role.has_value())
            {
                return false;
            }
            if (r.type() == role.type() && eq(r, role))
            {
                return true;
            }
        }

        return false;
    }

    virtual bool eq(const std::any &le, const std::any &ri) const = 0;

    const std::string &name() const
    {
        return name_;
    }

private:
    std::vector<std::any> roles_;
    const std::string name_;
};
