#ifndef UTIL_STATUS_H
#define UTIL_STATUS_H

#include <memory>
#include <string>

namespace util {

enum StatusCode { UNKNOWN, OK, INVALID_ARGUMENT };

class Status {
public:
  Status() : state_(nullptr) {}
  Status(StatusCode c) : Status(c, std::string()) {}
  Status(StatusCode c, std::string m);
  Status(const Status &s);

  inline StatusCode Code() const { return ok() ? StatusCode::OK : state_->code; }
  inline std::string Message() const { return ok() ? std::string() : state_->msg; }

  static Status OK() { return Status(); }

  bool ok() const { return (state_ == NULL); }

private:
  struct State {
    StatusCode code;
    std::string msg;
  };
  std::unique_ptr<State> state_;
};

} // namespace util

#endif // UTIL_STATUS_H