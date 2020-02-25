#ifndef UTIL_STATUS_OR_H
#define UTIL_STATUS_OR_H

#include <optional>

#include "util/status.h"

namespace util {

template <typename T> class StatusOr {
  // T value;
  std::optional<T> value_;
  Status status_;

public:
  explicit StatusOr();
  StatusOr(const StatusOr &) = default;
  StatusOr &operator=(const StatusOr &) = default;
  StatusOr(StatusOr &&) = default;
  StatusOr &operator=(StatusOr &&) = default;
  template <typename U, typename std::enable_if<
                            std::is_convertible<U, T>::value>::type * = nullptr>
  StatusOr(const StatusOr<U> &other);
  template <typename U, typename std::enable_if<
                            std::is_convertible<U, T>::value>::type * = nullptr>
  StatusOr(StatusOr<U> &&other);
  template <typename U, typename std::enable_if<
                            std::is_convertible<U, T>::value>::type * = nullptr>
  StatusOr &operator=(const StatusOr<U> &other);
  template <typename U, typename std::enable_if<
                            std::is_convertible<U, T>::value>::type * = nullptr>
  StatusOr &operator=(StatusOr<U> &&other);
  StatusOr(const T &v);
  StatusOr(const Status &status);
  StatusOr &operator=(const Status &status);
  StatusOr(T &&value);
  StatusOr(Status &&status);
  StatusOr &operator=(Status &&status);

  bool ok() const { return this->status_.ok(); }

  const Status &status() const &;
  Status status() &&;

  const T &ValueOrDie() const &;
  T &ValueOrDie() &;
  const T &&ValueOrDie() const &&;
  T &&ValueOrDie() &&;

  // void IgnoreError() const;
};

} // namespace util

#endif // UTIL_STATUS_OR_H