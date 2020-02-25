#ifndef UTIL_STATUS_H
#define UTIL_STATUS_H

#include <string>

namespace util {

enum StatusCode { UNKNOWN, OK, INVALID_ARGUMENT };

class Status {
  StatusCode code;
  std::string msg;

public:
  Status(StatusCode c) : code(c) {}
  Status(StatusCode c, std::string m) : code(c), msg(m) {}
  inline StatusCode Code() const { return code; }
  inline std::string Message() const { return msg; }
  
  bool ok() const;
};

} // namespace util

#endif // UTIL_STATUS_H