/*
 * @Author: Leo
 * @Date: 2022-02-01 21:47:19
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-21 07:58:03
 */
#pragma once
#ifndef YUZHI_LEDGERDB_ERROR
#define YUZHI_LEDGERDB_ERROR
#include <ostream>
#include <string>
#include <optional>

namespace yuzhi::common {
class Error {
 public:
  Error(const std::string &message) : message_(message) {}
  virtual ~Error() = default;
  Error &operator=(const Error &other) = default;

  virtual const std::string &message() const { return message_; }

  friend std::ostream &operator<<(std::ostream &out, const Error &e) {
    out << "Error:" << e.message();
    return out;
  }

  bool operator == (const std::optional<Error>& error)
  {
        if(!error.has_value())
        {
            return false;
        }

        return *this == error.value();
  }

  bool eq (Error (*f)())
  {
        return this->message_ == f().message_;
  }

 public:
  static inline Error InvalidLedger() { return Error("InvalidLedger"); };
  static inline Error MerkleTreeUpdateError() {
    return Error("MerkleTreeUpdateError");
  };
  static inline Error InvalidMerkleTree() {
    return Error("InvalidMerkleTree");
  };
  static inline Error SerializeError() { return Error("SerializeError"); };
  static inline Error DeserializeError() { return Error("DeserializeError"); };
  static inline Error HashError() { return Error("HashError"); };
  static inline Error UnLeader() { return Error("Leader unavailable"); };
  static inline Error Redirect() { return Error("redirect"); };
  static inline Error RaftError() { return Error("RaftError"); };
  static inline Error UndefineGammarError() {return Error("Undefine Gammar");};
  static inline Error RepeatKey() {return Error("Repeat Key Exist"); }
  static inline Error InvalidKey() {return Error("Invalid Key"); }
  static inline Error InvalidValue() {return Error("Invalid Value"); }
  static inline Error InvalidType() {return Error("Invalid Type"); }
  static inline Error InternalError(const std::string& msg) {return Error(msg); }
 private:
  const std::string message_;
};


inline bool operator == (const Error& r, const std::optional<Error>& l)
{
    return l == r;
}

}

#endif  // YUZHI_LEDGERDB_ERROR