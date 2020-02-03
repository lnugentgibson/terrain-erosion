#ifndef UTIL_STATUS_OR_H
#define UTIL_STATUS_OR_H

#include <optional>

#include "util/status.h"

namespace util {

template <typename T> class StatusOr {
  // T value;
  std::optional<T> value;
  Status status;

public:
  StatusOr(T v) : value(v), status(OK) {}
  StatusOr(Status s) : status(s) {}
  inline Status State() const { return status; }
  inline bool HasValue() const { return value.has_value(); }
  inline operator bool() const { return value.has_value(); }
  inline const T &Value() const { return *value; }
  inline const T &operator*() const & { return *value; }
  // inline const T&& operator *() const&& { return value; }
  // inline const T* operator ->() const { return &value; }
};

} // namespace util

#endif // UTIL_STATUS_OR_H