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
  explicit StatusOr() : StatusOr(Status(StatusCode::UNKNOWN, ""));
  StatusOr(const StatusOr &other) {
    if (other.ok()) {
      MakeValue(other.data_);
      MakeStatus();
    } else {
      MakeStatus(other.status_);
    }
  }
  StatusOr &operator=(const StatusOr &other) {
    if (this == &other)
      return *this;
    if (other.ok())
      Assign(other.data_);
    else
      Assign(other.status_);
    return *this;
  }
  StatusOr(StatusOr &&other) {
    if (other.ok()) {
      MakeValue(std::move(other.data_));
      MakeStatus();
    } else {
      MakeStatus(other.status_);
    }
  }
  StatusOr &operator=(StatusOr &&other) {
    if (this == &other)
      return *this;
    if (other.ok())
      Assign(std::move(other.data_));
    else
      Assign(std::move(other.status_));
    return *this;
  }
  template <typename U> StatusOr(const StatusOr<U> &other) {
    if (other.ok()) {
      MakeValue(other.data_);
      MakeStatus();
    } else {
      MakeStatus(other.status_);
    }
  }
  template <typename U> StatusOr(StatusOr<U> &&other) {
    if (other.ok()) {
      MakeValue(std::move(other.data_));
      MakeStatus();
    } else {
      MakeStatus(other.status_);
    }
  }
  template <typename U> StatusOr &operator=(const StatusOr<U> &other);
  template <typename U> StatusOr &operator=(StatusOr<U> &&other);
  StatusOr(const T &v) : data_(value) { MakeStatus(); }
  StatusOr(const Status &status) : status_(status) { EnsureNotOk(); }
  StatusOr &operator=(const Status &status);
  StatusOr(T &&value) : data_(std::move(value)) { MakeStatus(); }
  StatusOr(Status &&status) : status_(std::move(status)) { EnsureNotOk(); }
  StatusOr &operator=(Status &&status);

  ~StatusOr() {
    if (ok()) {
      status_.~Status();
      data_.~T();
    } else {
      status_.~Status();
    }
  }

  bool ok() const { return this->status_.ok(); }

  const Status &status() const &;
  Status status() &&;

  const T &ValueOrDie() const &;
  T &ValueOrDie() &;
  const T &&ValueOrDie() const &&;
  T &&ValueOrDie() &&;

  void IgnoreError() const;

  void Assign(const T &value) {
    if (ok()) {
      data_.~T();
      MakeValue(value);
    } else {
      MakeValue(value);
      status_ = Status::OK();
    }
  }

  void Assign(T &&value) {
    if (ok()) {
      data_.~T();
      MakeValue(std::move(value));
    } else {
      MakeValue(std::move(value));
      status_ = Status::OK();
    }
  }

  void Assign(const Status &status) {
    Clear();
    status_ = status;
    EnsureNotOk();
  }

  void Assign(Status &&status) {
    Clear();
    status_ = status;
    EnsureNotOk();
  }

private:
  Status status_;

  struct Dummy {};
  union {
    Dummy dummy_;
    T data_;
  };

  void Clear() {
    if (ok()) data_.~T();
  }

  void EnsureOk() const {
    if (!ok()) Helper::Crash(status_);
  }

  void EnsureNotOk() {
    if (ok()) Helper::HandleInvalidStatusCtorArg(&status_);
  }

  template <typename Arg>
  void MakeValue(Arg&& arg) {
    internal_statusor::PlacementNew<T>(&dummy_, std::forward<Arg>(arg));
  }

  template <typename... Args>
  void MakeStatus(Args&&... args) {
    internal_statusor::PlacementNew<Status>(&status_,
                                            std::forward<Args>(args)...);
  }
};

} // namespace util

#endif // UTIL_STATUS_OR_H