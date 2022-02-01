#pragma once

#include <string>
#include <ostream>

class Error
{
public:
    Error(const std::string &message) : message_(message) {}
    virtual ~Error() = default;
    Error &operator=(const Error &other) = default;

    virtual const std::string &message() const { return message_; }

    friend std::ostream &operator<<(std::ostream &out, const Error &e)
    {
        out << "Error:" << e.message();
        return out;
    }

public:
    static inline Error InvalidLedger() { return Error("InvalidLedger"); };
    static inline Error MerkleTreeUpdateError() { return Error("MerkleTreeUpdateError"); };
    static inline Error InvalidMerkleTree() { return Error("InvalidMerkleTree"); };
    static inline Error SerializeError() { return Error("SerializeError"); };
    static inline Error DeserializeError() { return Error("DeserializeError"); };
    static inline Error HashError() { return Error("HashError"); };

private:
    std::string message_;
};
