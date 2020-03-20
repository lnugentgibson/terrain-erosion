#ifndef EXPERIMENTAL_USERS_NUGENTGIBSON_MATH_VECTOR_VECTOR_H_
#define EXPERIMENTAL_USERS_NUGENTGIBSON_MATH_VECTOR_VECTOR_H_

#include <algorithm>
#include <sstream>
#include <string>

#include "util/status.h"
#include "util/statusor.h"

namespace math {
namespace vector {

class Matrix;
class BVector;
class IVector;
class FVector;
class DVector;

using Vector = FVector;

#define VECTOR_CONSTRUCTORS(VectorType, DataType)                              \
  explicit VectorType(int d) : d_(d), c_(new DataType[d]) { fill(0); }         \
  VectorType(int d, DataType *c) : d_(d), c_(new DataType[d]) { set(c); }      \
  template <typename T> VectorType(int d, T *c) : d_(d), c_(new DataType[d]) { \
    set(c);                                                                    \
  }                                                                            \
  VectorType(DataType x, DataType y) : d_(2), c_(new DataType[2]) {            \
    c_[0] = x;                                                                 \
    c_[1] = y;                                                                 \
  }                                                                            \
  VectorType(DataType x, DataType y, DataType z)                               \
      : d_(3), c_(new DataType[3]) {                                           \
    c_[0] = x;                                                                 \
    c_[1] = y;                                                                 \
    c_[2] = z;                                                                 \
  }                                                                            \
  VectorType(DataType x, DataType y, DataType z, DataType w)                   \
      : d_(4), c_(new DataType[4]) {                                           \
    c_[0] = x;                                                                 \
    c_[1] = y;                                                                 \
    c_[2] = z;                                                                 \
    c_[3] = w;                                                                 \
  }                                                                            \
  VectorType(const VectorType &v) : VectorType(v.d_, v.c_) {}                  \
  VectorType(VectorType &&v) : d_(v.d_), c_(v.c_) { v.c_ = nullptr; }          \
  VectorType &operator=(const VectorType &v) {                                 \
    if (v.d_ == d_) {                                                          \
      set(v.c_);                                                               \
    }                                                                          \
    return *this;                                                              \
  }                                                                            \
  VectorType &operator=(VectorType &&v) {                                      \
    if (v.d_ == d_) {                                                          \
      if (c_ != nullptr)                                                       \
        delete[] c_;                                                           \
      c_ = v.c_;                                                               \
      v.c_ = nullptr;                                                          \
    }                                                                          \
    return *this;                                                              \
  }                                                                            \
  ~VectorType() {                                                              \
    if (c_ != nullptr)                                                         \
      delete[] c_;                                                             \
  }

#define VECTOR_INVALID_CONSTRUCTOR(VectorType)                                 \
  explicit VectorType(invalid_vector_t tag) : d_(0), c_(nullptr) {}

#define VECTOR_CONVERSION_CONSTRUCTOR_SIG(VectorType, OtherVectorType)         \
  explicit VectorType(const OtherVectorType &v);                               \
  explicit VectorType(OtherVectorType &&v);                                    \
  VectorType &operator=(const OtherVectorType &v);                             \
  VectorType &operator=(OtherVectorType &&v);

#define VECTOR_INDEXERS_ITERATORS(VectorType, DataType)                        \
  inline size_t dimension() const { return d_; }                               \
  inline DataType operator[](int i) const { return c_[i]; }                    \
  /* inline DataType operator[](size_t i) const { return c[i]; } */            \
  inline DataType &operator[](int i) { return c_[i]; }                         \
  /* inline DataType &operator[](size_t i) { return c[i]; } */                 \
  template <typename F> void forEach(F f) const {                              \
    DataType *A = new DataType[d_];                                            \
    std::copy(c_, c_ + d_, A);                                                 \
    for (int i = 0; i < (int)d_; i++) {                                        \
      f(c_[i], i, A);                                                          \
    }                                                                          \
    delete[] A;                                                                \
  }                                                                            \
  template <typename F> VectorType map(F f) const {                            \
    DataType *A = new DataType[d_];                                            \
    std::copy(c_, c_ + d_, A);                                                 \
    VectorType v(d_);                                                          \
    for (int i = 0; i < (int)d_; i++) {                                        \
      v[i] = f(c_[i], i, A);                                                   \
    }                                                                          \
    delete[] A;                                                                \
    return v;                                                                  \
  }                                                                            \
  template <typename F> VectorType &transform(F f) {                           \
    DataType *A = new DataType[d_];                                            \
    std::copy(c_, c_ + d_, A);                                                 \
    for (int i = 0; i < (int)d_; i++) {                                        \
      c_[i] = f(c_[i], i, A);                                                  \
    }                                                                          \
    delete[] A;                                                                \
    return *this;                                                              \
  }                                                                            \
  template <typename F, typename S> S reduce(F f, S s) const {                 \
    DataType *A = new DataType[d_];                                            \
    std::copy(c_, c_ + d_, A);                                                 \
    S a = s;                                                                   \
    for (int i = 0; i < (int)d_; i++) {                                        \
      a = f(a, c_[i], i, A);                                                   \
    }                                                                          \
    delete[] A;                                                                \
    return a;                                                                  \
  }                                                                            \
  template <typename F> bool every(F f) const {                                \
    DataType *A = new DataType[d_];                                            \
    std::copy(c_, c_ + d_, A);                                                 \
    for (int i = 0; i < (int)d_; i++) {                                        \
      if (!f(c_[i], i, A))                                                     \
        return false;                                                          \
    }                                                                          \
    delete[] A;                                                                \
    return true;                                                               \
  }                                                                            \
  template <typename F> bool some(F f) const {                                 \
    DataType *A = new DataType[d_];                                            \
    std::copy(c_, c_ + d_, A);                                                 \
    for (int i = 0; i < (int)d_; i++) {                                        \
      if (f(c_[i], i, A))                                                      \
        return true;                                                           \
    }                                                                          \
    delete[] A;                                                                \
    return false;                                                              \
  }                                                                            \
  DataType *toArray() const {                                                  \
    DataType *A = new DataType[d_];                                            \
    std::copy(c_, c_ + (int)d_, A);                                            \
    return A;                                                                  \
  }                                                                            \
  void toArray(DataType *A) const { std::copy(c_, c_ + d_, A); }               \
  VectorType clone() const { return VectorType(d_, toArray()); }               \
  void set(DataType *c) { std::copy(c, c + d_, c_); }                          \
  template <typename T> void set(T *c) {                                       \
    for (int i = 0; i < (int)d_; i++) {                                        \
      c_[i] = static_cast<DataType>(c[i]);                                     \
    }                                                                          \
  }                                                                            \
  void fill(DataType c) {                                                      \
    for (int i = 0; i < (int)d_; i++) {                                        \
      c_[i] = c;                                                               \
    }                                                                          \
  }                                                                            \
  std::string toStr() const {                                                  \
    std::stringstream ss;                                                      \
    ss << #VectorType << d_ << '[';                                            \
    ss << c_[0];                                                               \
    for (int i = 1; i < (int)d_; i++) {                                        \
      ss << ',' << c_[i];                                                      \
    }                                                                          \
    ss << ']';                                                                 \
    return ss.str();                                                           \
  }

#define VECTOR_COMP_SIGS(ParamVectorType, DataType)                            \
  BVector operator<(const ParamVectorType &v) const;                           \
  util::StatusOr<BVector> lessThan(const ParamVectorType &v) const;            \
  BVector operator<=(const ParamVectorType &v) const;                          \
  util::StatusOr<BVector> lessThanOrEqual(const ParamVectorType &v) const;     \
  bool operator==(const ParamVectorType &v) const;                             \
  bool equals(const ParamVectorType &v, DataType tolerance = .0001) const;     \
  util::StatusOr<BVector> eachEquals(const ParamVectorType &v,                 \
                                     DataType tolerance = .0001) const;        \
  BVector operator>=(const ParamVectorType &v) const;                          \
  util::StatusOr<BVector> greaterThanOrEqual(const ParamVectorType &v) const;  \
  BVector operator>(const ParamVectorType &v) const;                           \
  util::StatusOr<BVector> greaterThan(const ParamVectorType &v) const;

#define VECTOR_UNARY_OP_INPLACE_SIG(VectorType, function)                      \
  VectorType &function();
#define VECTOR_UNARY_OP_OUTOFPLACE_SIG(VectorType, function)                   \
  VectorType function() const;
#define VECTOR_UNARY_OP_OUTOFPLACE_O_SIG(VectorType, op)                       \
  VectorType operator op() const;

#define VECTOR_UNARY_OP_SIG(VectorType, functionInPlace, functionOutOfPlace,   \
                            outOfPlaceOperator)                                \
  VECTOR_UNARY_OP_INPLACE_SIG(VectorType, functionInPlace)                     \
  VECTOR_UNARY_OP_OUTOFPLACE_SIG(VectorType, functionOutOfPlace)               \
  VECTOR_UNARY_OP_OUTOFPLACE_O_SIG(VectorType, outOfPlaceOperator)

#define VECTOR_UNARY_FN_S_SIG(function, ReturnType) ReturnType &function();
#define VECTOR_UNARY_FN_O_SIG(function, ReturnType) ReturnType function() const;

#define VECTOR_UNARY_FN_SIG(VectorType, functionInplace, functionOutofplace,   \
                            ReturnType)                                        \
  VECTOR_UNARY_FN_S_SIG(functionInplace, VectorType)                           \
  VECTOR_UNARY_FN_O_SIG(functionOutofplace, ReturnType)

#define VECTOR_UNARY_FN_SIGS(VectorType, FloatVectorType, IntegerVectorType)   \
  VECTOR_UNARY_FN_SIG(VectorType, cos, cos_of, FloatVectorType)                \
  VECTOR_UNARY_FN_SIG(VectorType, sin, sin_of, FloatVectorType)                \
  VECTOR_UNARY_FN_SIG(VectorType, tan, tan_of, FloatVectorType)                \
  VECTOR_UNARY_FN_SIG(VectorType, acos, acos_of, FloatVectorType)              \
  VECTOR_UNARY_FN_SIG(VectorType, asin, asin_of, FloatVectorType)              \
  VECTOR_UNARY_FN_SIG(VectorType, atan, atan_of, FloatVectorType)              \
  VECTOR_UNARY_FN_SIG(VectorType, cosh, cosh_of, FloatVectorType)              \
  VECTOR_UNARY_FN_SIG(VectorType, sinh, sinh_of, FloatVectorType)              \
  VECTOR_UNARY_FN_SIG(VectorType, tanh, tanh_of, FloatVectorType)              \
  VECTOR_UNARY_FN_SIG(VectorType, acosh, acosh_of, FloatVectorType)            \
  VECTOR_UNARY_FN_SIG(VectorType, asinh, asinh_of, FloatVectorType)            \
  VECTOR_UNARY_FN_SIG(VectorType, atanh, atanh_of, FloatVectorType)            \
  VECTOR_UNARY_FN_SIG(VectorType, floor, floor_of, IntegerVectorType)          \
  VECTOR_UNARY_FN_SIG(VectorType, ceil, ceil_of, IntegerVectorType)

#define VECTOR_BINARY_OP_INPLACE_SCALAR_SIG(VectorType, DataType, function)    \
  VectorType &function(const DataType v);
#define VECTOR_BINARY_OP_INPLACE_ARR_SIG(VectorType, DataType, function)       \
  VectorType &function(const DataType *v);
#define VECTOR_BINARY_OP_INPLACE_VEC_SIG(VectorType, ParamVectorType,          \
                                         function)                             \
  util::StatusOr<VectorType> function(const ParamVectorType &v);
#define VECTOR_BINARY_OP_INPLACE_O_SCALAR_SIG(VectorType, DataType, op)        \
  VectorType &operator op(const DataType v);
#define VECTOR_BINARY_OP_INPLACE_O_ARR_SIG(VectorType, DataType, op)           \
  VectorType &operator op(const DataType *v);
#define VECTOR_BINARY_OP_INPLACE_O_VEC_SIG(VectorType, ParamVectorType, op)    \
  VectorType &operator op(const ParamVectorType &v);
#define VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_SIG(VectorType, DataType, function) \
  VectorType function(const DataType v) const;
#define VECTOR_BINARY_OP_OUTOFPLACE_ARR_SIG(VectorType, DataType, function)    \
  VectorType function(const DataType *v) const;
#define VECTOR_BINARY_OP_OUTOFPLACE_VEC_SIG(VectorType, ParamVectorType,       \
                                            function)                          \
  util::StatusOr<VectorType> function(const ParamVectorType &v) const;
#define VECTOR_BINARY_OP_OUTOFPLACE_O_SCALAR_SIG(VectorType, DataType, op)     \
  VectorType operator op(const DataType v) const;
#define VECTOR_BINARY_OP_OUTOFPLACE_O_ARR_SIG(VectorType, DataType, op)        \
  VectorType operator op(const DataType *v) const;
#define VECTOR_BINARY_OP_OUTOFPLACE_O_VEC_SIG(VectorType, ParamVectorType, op) \
  VectorType operator op(const ParamVectorType &v) const;

#define VECTOR_BINARY_OP_INPLACE_SIG(VectorType, DataType, function)           \
  VECTOR_BINARY_OP_INPLACE_SCALAR_SIG(VectorType, DataType, function)          \
  VECTOR_BINARY_OP_INPLACE_ARR_SIG(VectorType, DataType, function)             \
  VECTOR_BINARY_OP_INPLACE_VEC_SIG(VectorType, VectorType, function)

#define VECTOR_BINARY_OP_INPLACE_O_SIG(VectorType, DataType, op)               \
  VECTOR_BINARY_OP_INPLACE_O_SCALAR_SIG(VectorType, DataType, op)              \
  VECTOR_BINARY_OP_INPLACE_O_ARR_SIG(VectorType, DataType, op)                 \
  VECTOR_BINARY_OP_INPLACE_O_VEC_SIG(VectorType, VectorType, op)

#define VECTOR_BINARY_OP_OUTOFPLACE_SIG(VectorType, DataType, function)        \
  VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_SIG(VectorType, DataType, function)       \
  VECTOR_BINARY_OP_OUTOFPLACE_ARR_SIG(VectorType, DataType, function)          \
  VECTOR_BINARY_OP_OUTOFPLACE_VEC_SIG(VectorType, VectorType, function)

#define VECTOR_BINARY_OP_OUTOFPLACE_O_SIG(VectorType, DataType, op)            \
  VECTOR_BINARY_OP_OUTOFPLACE_O_SCALAR_SIG(VectorType, DataType, op)           \
  VECTOR_BINARY_OP_OUTOFPLACE_O_ARR_SIG(VectorType, DataType, op)              \
  VECTOR_BINARY_OP_OUTOFPLACE_O_VEC_SIG(VectorType, VectorType, op)

#define VECTOR_BINARY_OP_SIG(VectorType, DataType, functionInPlace,            \
                             inPlaceOperator, functionOutOfPlace,              \
                             outOfPlaceOperator)                               \
  VECTOR_BINARY_OP_INPLACE_SIG(VectorType, DataType, functionInPlace)          \
  VECTOR_BINARY_OP_INPLACE_O_SIG(VectorType, DataType, inPlaceOperator)        \
  VECTOR_BINARY_OP_OUTOFPLACE_SIG(VectorType, DataType, functionOutOfPlace)    \
  VECTOR_BINARY_OP_OUTOFPLACE_O_SIG(VectorType, DataType, outOfPlaceOperator)

#define VECTOR_BINARY_OP_VEC_SIG(                                              \
    VectorType, ReturnVectorType, ParamVectorType, functionInPlace,            \
    inPlaceOperator, functionOutOfPlace, outOfPlaceOperaton)                   \
  VECTOR_BINARY_OP_INPLACE_VEC_SIG(VectorType, ParamVectorType,                \
                                   functionInPlace)                            \
  VECTOR_BINARY_OP_INPLACE_O_VEC_SIG(VectorType, ParamVectorType,              \
                                     inPlaceOperator)                          \
  VECTOR_BINARY_OP_OUTOFPLACE_VEC_SIG(ReturnVectorType, ParamVectorType,       \
                                      functionOutOfPlace)                      \
  VECTOR_BINARY_OP_OUTOFPLACE_O_VEC_SIG(ReturnVectorType, ParamVectorType,     \
                                        outOfPlaceOperaton)

#define VECTOR_BINARY_OP_ARITH_SIGS(VectorType, DataType)                      \
  VECTOR_BINARY_OP_SIG(VectorType, DataType, add, +=, sum, +)                  \
  VECTOR_BINARY_OP_SIG(VectorType, DataType, subtract, -=, difference, -)      \
  VECTOR_BINARY_OP_SIG(VectorType, DataType, multiply, *=, product, *)         \
  VECTOR_BINARY_OP_SIG(VectorType, DataType, divide, /=, quotient, /)

#define VECTOR_BINARY_OP_VEC_ARITH_SIGS(VectorType, ReturnVectorType,          \
                                        ParamVectorType)                       \
  VECTOR_BINARY_OP_VEC_SIG(VectorType, ReturnVectorType, ParamVectorType, add, \
                           +=, sum, +)                                         \
  VECTOR_BINARY_OP_VEC_SIG(VectorType, ReturnVectorType, ParamVectorType,      \
                           subtract, -=, difference, -)                        \
  VECTOR_BINARY_OP_VEC_SIG(VectorType, ReturnVectorType, ParamVectorType,      \
                           multiply, *=, product, *)                           \
  VECTOR_BINARY_OP_VEC_SIG(VectorType, ReturnVectorType, ParamVectorType,      \
                           divide, /=, quotient, /)

#define VECTOR_BINARY_OP_SIG_NOIOP(VectorType, DataType, functionInPlace,      \
                                   functionOutOfPlace, outOfPlaceOperator)     \
  VECTOR_BINARY_OP_INPLACE_SIG(VectorType, DataType, functionInPlace)          \
  VECTOR_BINARY_OP_OUTOFPLACE_SIG(VectorType, DataType, functionOutOfPlace)    \
  VECTOR_BINARY_OP_OUTOFPLACE_O_SIG(VectorType, DataType, outOfPlaceOperator)

#define VECTOR_BINARY_OP_LOGIC_SIGS(VectorType, DataType)                      \
  VECTOR_BINARY_OP_SIG_NOIOP(VectorType, DataType, bor, disjunction, ||)       \
  VECTOR_BINARY_OP_SIG_NOIOP(VectorType, DataType, band, conjunction, &&)      \
  VECTOR_BINARY_OP_INPLACE_O_VEC_SIG(VectorType, VectorType, +=)               \
  VECTOR_BINARY_OP_OUTOFPLACE_O_VEC_SIG(VectorType, VectorType, +)             \
  VECTOR_BINARY_OP_INPLACE_O_VEC_SIG(VectorType, VectorType, *=)               \
  VECTOR_BINARY_OP_OUTOFPLACE_O_VEC_SIG(VectorType, VectorType, *)

#define VECTOR_BINARY_FN_S_SCALAR_SIG(VectorType, DataType, function)          \
  VectorType &function(const DataType v);
#define VECTOR_BINARY_FN_S_ARR_SIG(VectorType, DataType, function)             \
  VectorType &function(const DataType *v);
#define VECTOR_BINARY_FN_S_VEC_SIG(VectorType, ParamVectorType, function)      \
  util::StatusOr<VectorType> function(const ParamVectorType &v);
#define VECTOR_BINARY_FN_SO_SCALAR_SIG(VectorType, DataType, op)               \
  VectorType &operator op(const DataType v);
#define VECTOR_BINARY_FN_SO_ARR_SIG(VectorType, DataType, op)                  \
  VectorType &operator op(const DataType *v);
#define VECTOR_BINARY_FN_SO_VEC_SIG(VectorType, ParamVectorType, op)           \
  VectorType &operator op(const ParamVectorType &v);
#define VECTOR_BINARY_FN_O_SCALAR_SIG(VectorType, DataType, function)          \
  VectorType function(const DataType v) const;
#define VECTOR_BINARY_FN_O_ARR_SIG(VectorType, DataType, function)             \
  VectorType function(const DataType *v) const;
#define VECTOR_BINARY_FN_O_VEC_SIG(VectorType, ParamVectorType, function)      \
  util::StatusOr<VectorType> function(const ParamVectorType &v) const;
#define VECTOR_BINARY_FN_OO_SCALAR_SIG(VectorType, DataType, op)               \
  VectorType operator op(const DataType v) const;
#define VECTOR_BINARY_FN_OO_ARR_SIG(VectorType, DataType, op)                  \
  VectorType operator op(const DataType *v) const;
#define VECTOR_BINARY_FN_OO_VEC_SIG(VectorType, ParamVectorType, op)           \
  VectorType operator op(const ParamVectorType &v) const;

#define VECTOR_BINARY_FN_S_SIG(VectorType, DataType, function)                 \
  VECTOR_BINARY_FN_S_SCALAR_SIG(VectorType, DataType, function)                \
  VECTOR_BINARY_FN_S_ARR_SIG(VectorType, DataType, function)                   \
  VECTOR_BINARY_FN_S_VEC_SIG(VectorType, VectorType, function)

#define VECTOR_BINARY_FN_SO_SIG(VectorType, DataType, op)                      \
  VECTOR_BINARY_FN_SO_SCALAR_SIG(VectorType, DataType, op)                     \
  VECTOR_BINARY_FN_SO_ARR_SIG(VectorType, DataType, op)                        \
  VECTOR_BINARY_FN_SO_VEC_SIG(VectorType, VectorType, op)

#define VECTOR_BINARY_FN_O_SIG(VectorType, DataType, function)                 \
  VECTOR_BINARY_FN_O_SCALAR_SIG(VectorType, DataType, function)                \
  VECTOR_BINARY_FN_O_ARR_SIG(VectorType, DataType, function)                   \
  VECTOR_BINARY_FN_O_VEC_SIG(VectorType, VectorType, function)

#define VECTOR_BINARY_FN_OO_SIG(VectorType, DataType, op)                      \
  VECTOR_BINARY_FN_OO_SCALAR_SIG(VectorType, DataType, op)                     \
  VECTOR_BINARY_FN_OO_ARR_SIG(VectorType, DataType, op)                        \
  VECTOR_BINARY_FN_OO_VEC_SIG(VectorType, VectorType, op)

#define VECTOR_BINARY_FN_SIG(VectorType, DataType, functionInPlace,            \
                             inPlaceOperator, functionOutOfPlace,              \
                             outOfPlaceOperator)                               \
  VECTOR_BINARY_FN_S_SIG(VectorType, DataType, functionInPlace)                \
  VECTOR_BINARY_FN_SO_SIG(VectorType, DataType, inPlaceOperator)               \
  VECTOR_BINARY_FN_O_SIG(VectorType, DataType, functionOutOfPlace)             \
  VECTOR_BINARY_FN_OO_SIG(VectorType, DataType, outOfPlaceOperator)

#define VECTOR_BINARY_FN_VEC_SIG(                                              \
    VectorType, ReturnVectorType, ParamVectorType, functionInPlace,            \
    inPlaceOperator, functionOutOfPlace, outOfPlaceOperaton)                   \
  VECTOR_BINARY_FN_S_VEC_SIG(VectorType, ParamVectorType, functionInPlace)     \
  VECTOR_BINARY_FN_SO_VEC_SIG(VectorType, ParamVectorType, inPlaceOperator)    \
  VECTOR_BINARY_FN_O_VEC_SIG(ReturnVectorType, ParamVectorType,                \
                             functionOutOfPlace)                               \
  VECTOR_BINARY_FN_OO_VEC_SIG(ReturnVectorType, ParamVectorType,               \
                              outOfPlaceOperaton)

#define VECTOR_BINARY_MISC_SIGS(VectorType, ReturnVectorType, ParamVectorType, \
                                DataType, FloatType)                           \
  util::StatusOr<DataType> dot(const ParamVectorType &v) const;                \
  FloatType angleTo(const ParamVectorType &a) const;                           \
  util::StatusOr<ReturnVectorType> crossEquals(const ParamVectorType *vs);     \
  util::StatusOr<ReturnVectorType> cross(const ParamVectorType *vs) const;     \
  util::StatusOr<ReturnVectorType> min(const ParamVectorType &v) const;        \
  util::StatusOr<ReturnVectorType> max(const ParamVectorType &v) const;

#define VECTOR_NORM_SIGS(VectorType, FloatVectorType, DataType, FloatType)     \
  FloatType length2() const;                                                   \
  FloatType length() const;                                                    \
  DataType manhattan() const;                                                  \
  FloatType pnorm(int p) const;                                                \
  FloatType pnormn(int p, int n) const;                                        \
  VectorType &normalize();                                                     \
  FloatVectorType unit() const;                                                \
  DataType min() const;                                                        \
  DataType max() const;

struct invalid_vector_t {};

class BVector {
public:
  VECTOR_CONSTRUCTORS(BVector, bool)

  VECTOR_CONVERSION_CONSTRUCTOR_SIG(BVector, IVector)
  VECTOR_CONVERSION_CONSTRUCTOR_SIG(BVector, FVector)
  VECTOR_CONVERSION_CONSTRUCTOR_SIG(BVector, DVector)

  VECTOR_INDEXERS_ITERATORS(BVector, bool)

  VECTOR_UNARY_OP_SIG(BVector, bnot, negation, !)
  VECTOR_UNARY_OP_OUTOFPLACE_O_SIG(BVector, -)

  VECTOR_BINARY_OP_LOGIC_SIGS(BVector, bool)

  bool every() {
    for (int i = 0; i < (int)d_; i++) {
      if (!c_[i]) {
        return false;
      }
    }
    return true;
  }

  bool some() {
    for (int i = 0; i < (int)d_; i++) {
      if (c_[i]) {
        return true;
      }
    }
    return false;
  }

  friend IVector;
  friend FVector;
  friend DVector;

private:
  const size_t d_;
  bool *c_;

  VECTOR_INVALID_CONSTRUCTOR(BVector)
};

class IVector {
public:
  VECTOR_CONSTRUCTORS(IVector, int)

  VECTOR_CONVERSION_CONSTRUCTOR_SIG(IVector, FVector)
  VECTOR_CONVERSION_CONSTRUCTOR_SIG(IVector, DVector)

  VECTOR_INDEXERS_ITERATORS(IVector, int)

  VECTOR_UNARY_OP_SIG(IVector, negate, negation, -)

  VECTOR_UNARY_FN_SIGS(IVector, FVector, IVector)

  VECTOR_BINARY_OP_ARITH_SIGS(IVector, int)
  VECTOR_BINARY_OP_VEC_ARITH_SIGS(IVector, FVector, FVector)
  VECTOR_BINARY_OP_VEC_ARITH_SIGS(IVector, DVector, DVector)

  VECTOR_BINARY_OP_SIG(IVector, int, mod, %=, modulus, %)
  VECTOR_BINARY_FN_VEC_SIG(IVector, FVector, FVector, mod, %=, modulus, %)
  VECTOR_BINARY_FN_VEC_SIG(IVector, DVector, DVector, mod, %=, modulus, %)

  VECTOR_COMP_SIGS(IVector, float)
  // VECTOR_COMP_SIGS(FVector, float)
  // VECTOR_COMP_SIGS(DVector, double)

  VECTOR_BINARY_MISC_SIGS(IVector, IVector, IVector, int, float)

  VECTOR_NORM_SIGS(IVector, FVector, int, float)

  float *toAngles();

  friend BVector;
  friend FVector;
  friend DVector;

private:
  const size_t d_;
  int *c_;

  VECTOR_INVALID_CONSTRUCTOR(IVector)
};

class FVector {
public:
  VECTOR_CONSTRUCTORS(FVector, float)

  VECTOR_CONVERSION_CONSTRUCTOR_SIG(FVector, IVector)
  VECTOR_CONVERSION_CONSTRUCTOR_SIG(FVector, DVector)

  VECTOR_INDEXERS_ITERATORS(FVector, float)

  VECTOR_UNARY_OP_SIG(FVector, negate, negation, -)

  VECTOR_UNARY_FN_SIGS(FVector, FVector, IVector)

  VECTOR_BINARY_OP_ARITH_SIGS(FVector, float)
  VECTOR_BINARY_OP_VEC_ARITH_SIGS(FVector, FVector, IVector)
  VECTOR_BINARY_OP_VEC_ARITH_SIGS(FVector, DVector, DVector)

  VECTOR_BINARY_FN_SIG(FVector, float, mod, %=, modulus, %)
  VECTOR_BINARY_FN_VEC_SIG(FVector, FVector, IVector, mod, %=, modulus, %)
  VECTOR_BINARY_FN_VEC_SIG(FVector, DVector, DVector, mod, %=, modulus, %)

  // VECTOR_COMP_SIGS(IVector, float)
  VECTOR_COMP_SIGS(FVector, float)
  // VECTOR_COMP_SIGS(DVector, double)

  VECTOR_BINARY_MISC_SIGS(FVector, FVector, FVector, float, float)

  VECTOR_NORM_SIGS(FVector, FVector, float, float)

  float *toAngles();

  static FVector fromAngles(int d, float *as);
  static FVector fromAngles(int d, float m, float *as);
  static FVector randomDirection(int d);
  static FVector lerp(const FVector &v, const FVector &w, float factor);

  friend BVector;
  friend IVector;
  friend DVector;

private:
  const size_t d_;
  float *c_;

  VECTOR_INVALID_CONSTRUCTOR(FVector)
};

class DVector {
public:
  VECTOR_CONSTRUCTORS(DVector, double)

  VECTOR_CONVERSION_CONSTRUCTOR_SIG(DVector, IVector)
  VECTOR_CONVERSION_CONSTRUCTOR_SIG(DVector, FVector)

  VECTOR_INDEXERS_ITERATORS(DVector, double)

  VECTOR_UNARY_OP_SIG(DVector, negate, negation, -)

  VECTOR_UNARY_FN_SIGS(DVector, DVector, IVector)

  VECTOR_BINARY_OP_ARITH_SIGS(DVector, double)
  VECTOR_BINARY_OP_VEC_ARITH_SIGS(DVector, DVector, IVector)
  VECTOR_BINARY_OP_VEC_ARITH_SIGS(DVector, DVector, FVector)

  VECTOR_BINARY_FN_SIG(DVector, double, mod, %=, modulus, %)
  VECTOR_BINARY_FN_VEC_SIG(DVector, DVector, IVector, mod, %=, modulus, %)
  VECTOR_BINARY_FN_VEC_SIG(DVector, DVector, FVector, mod, %=, modulus, %)

  // VECTOR_COMP_SIGS(IVector, double)
  // VECTOR_COMP_SIGS(FVector, double)
  VECTOR_COMP_SIGS(DVector, double)

  VECTOR_BINARY_MISC_SIGS(DVector, DVector, DVector, double, double)

  VECTOR_NORM_SIGS(DVector, DVector, double, double)

  double *toAngles();

  static DVector fromAngles(int d, double *as);
  static DVector fromAngles(int d, float m, float *as);
  static DVector randomDirection(int d);
  static DVector lerp(const DVector &v, const DVector &w, double factor);

  friend BVector;
  friend IVector;
  friend FVector;

private:
  const size_t d_;
  double *c_;

  VECTOR_INVALID_CONSTRUCTOR(DVector)
};

void floor(FVector *v);
FVector floor(const FVector &v);
FVector &&floor(FVector &&v);
FVector &floor(const FVector &v, FVector *w);

} // namespace vector
} // namespace math

#endif // EXPERIMENTAL_USERS_NUGENTGIBSON_MATH_VECTOR_VECTOR_H_
