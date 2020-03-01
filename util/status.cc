#include "util/status.h"

#include <cassert>

namespace util {

const std::string &Status::empty_string() {
  static std::string *empty = new std::string;
  return *empty;
}

std::string error_name(StatusCode code) {
  switch (code) {
  case StatusCode::UNKNOWN:
    return "Unknown";
    break;
  case StatusCode::OK:
    return "OK";
    break;
  case StatusCode::INVALID_ARGUMENT:
    return "Invalid argument";
    break;
  case StatusCode::OUT_OF_RANGE:
    return "Out of range";
    break;
  case StatusCode::FAILED_PRECONDITION:
    return "Failed precondition";
    break;
  case StatusCode::NOT_FOUND:
    return "Not found";
    break;
  case StatusCode::ALREADY_EXISTS:
    return "Already exists";
    break;
  case StatusCode::PERMISSION_DENIED:
    return "Permission denied";
    break;
  case StatusCode::DEADLINE_EXCEEDED:
    return "Deadline exceeded";
    break;
  case StatusCode::UNAVAILABLE:
    return "Unavailable";
    break;
  case StatusCode::UNIMPLEMENTED:
    return "Unimplemented";
    break;
  case StatusCode::INTERNAL:
    return "Internal";
    break;
  case StatusCode::CANCELLED:
    return "Cancelled";
    break;
  /*
  case StatusCode::UNAUTHENTICATED:
    return "Unauthenticated";
    break;
  case StatusCode::RESOURCE_EXHAUSTED:
    return "Resource exhausted";
    break;
  case StatusCode::ABORTED:
    return "Aborted";
    break;
  case StatusCode::DATA_LOSS:
    return "Data loss";
    break;
  */
  default:
    char tmp[30];
    snprintf(tmp, sizeof(tmp), "Unknown code(%d)", static_cast<int>(code));
    return tmp;
    break;
  }
}

Status::Status(StatusCode c, std::string m) : state_(new State) {
  assert(c != StatusCode::OK);
  state_->code = c;
  state_->msg = m;
}

Status::Status(const Status &s) : state_(new State) {
  state_->code = s.state_->code;
  state_->msg = s.state_->msg;
}

inline Status &Status::operator=(const Status &s) {
  if (state_ != s.state_) {
    if (!s.state_) {
      state_ = nullptr;
    } else {
      state_ = std::unique_ptr<State>(new State(*s.state_));
    }
  }
  return *this;
}

inline bool Status::operator==(const Status &x) const {
  return (this->state_ == x.state_) || (ToString() == x.ToString());
}

inline bool Status::operator!=(const Status &x) const { return !(*this == x); }

std::string Status::ToString() const {
  if (state_ == nullptr) {
    return "OK";
  } else {
    std::string result(error_name(code()));
    result += ": ";
    result += state_->msg;
    return result;
  }
}

void Status::IgnoreError() const {
  // no-op
}

std::ostream &operator<<(std::ostream &os, const Status &x) {
  os << x.ToString();
  return os;
}

} // namespace util