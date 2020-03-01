#ifndef UTIL_STATUS_H
#define UTIL_STATUS_H

#include <memory>
#include <stdio.h>
#include <string>

namespace util {

enum StatusCode {
  UNKNOWN,
  OK,
  INVALID_ARGUMENT,
  OUT_OF_RANGE,
  FAILED_PRECONDITION,
  NOT_FOUND,
  ALREADY_EXISTS,
  PERMISSION_DENIED,
  DEADLINE_EXCEEDED,
  UNAVAILABLE,
  UNIMPLEMENTED,
  INTERNAL,
  CANCELLED
};

class Status {
public:
  Status() : state_(nullptr) {}
  Status(StatusCode c) : Status(c, std::string()) {}
  Status(StatusCode c, std::string m);
  Status(const Status &s);

  Status &operator=(const Status &s);

  inline StatusCode Code() const {
    return ok() ? StatusCode::OK : state_->code;
  }
  inline std::string Message() const {
    return ok() ? std::string() : state_->msg;
  }

  static Status OK() { return Status(); }

  bool ok() const { return (state_ == NULL); }

  StatusCode code() const { return ok() ? StatusCode::OK : state_->code; }

  const std::string &error_message() const {
    return ok() ? empty_string() : state_->msg;
  }

  bool operator==(const Status &x) const;
  bool operator!=(const Status &x) const;

  std::string ToString() const;

  void IgnoreError() const;

private:
  static const std::string &empty_string();

  struct State {
    StatusCode code;
    std::string msg;
  };
  std::unique_ptr<State> state_;
};

std::ostream &operator<<(std::ostream &os, const Status &x);

} // namespace util

#endif // UTIL_STATUS_H