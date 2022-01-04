#pragma once

#include <string>
#include <ostream>


class Error
{
public:
    Error(const std::string &message) : message_(message) {}
    virtual ~Error() = default;
    Error& operator=(const Error &other) = default;

    virtual const std::string &message() const { return message_; }

    friend std::ostream &operator<<(std::ostream &out, const Error &e)
    {
        out << "Error:" << e.message();
        return out;
    }

private:
    std::string message_;
};

