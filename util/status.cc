#include "util/status.h"

namespace util {

Status::Status(StatusCode c, std::string m) : state_(new State) {
  state_->code = c;
  state_->msg = m;
}

Status::Status(const Status &s) : state_(new State) {
  state_->code = s.state_->code;
  state_->msg = s.state_->msg;
}

} // namespace util