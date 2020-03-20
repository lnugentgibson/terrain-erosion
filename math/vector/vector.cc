#include "math/vector/vector.h"

#include <cmath>

#include "math/common.h"

namespace math {
namespace vector {

#define VECTOR_CONVERSION_CONSTRUCTOR(VectorType, OtherVectorType)             \
  VectorType::VectorType(const OtherVectorType &v) : VectorType(v.d_, v.c_) {} \
  VectorType::VectorType(OtherVectorType &&v) : VectorType(v.d_, v.c_) {       \
    delete[] v.c_;                                                             \
    v.c_ = nullptr;                                                            \
  }                                                                            \
  VectorType &VectorType::operator=(const OtherVectorType &v) {                \
    if (v.d_ == d_) {                                                          \
      set(v.c_);                                                               \
    }                                                                          \
    return *this;                                                              \
  }                                                                            \
  VectorType &VectorType::operator=(OtherVectorType &&v) {                     \
    if (v.d_ == d_) {                                                          \
      set(v.c_);                                                               \
      delete[] v.c_;                                                           \
      v.c_ = nullptr;                                                          \
    }                                                                          \
    return *this;                                                              \
  }

#define VECTOR_COMP(VectorType, ParamVectorType, DataType, FloatType)         \
  BVector VectorType::operator<(const ParamVectorType &v) const {             \
    if (v.d_ != d_) {                                                         \
      return BVector(invalid_vector_t());                                     \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = c_[i] < v.c_[i];                                                 \
    }                                                                         \
    return o;                                                                 \
  }                                                                           \
  util::StatusOr<BVector> VectorType::lessThan(const ParamVectorType &v)      \
      const {                                                                 \
    if (v.d_ != d_) {                                                         \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,                 \
                          "dimensions are not equal");                        \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = c_[i] < v.c_[i];                                                 \
    }                                                                         \
    return o;                                                                 \
  }                                                                           \
  BVector VectorType::operator<=(const ParamVectorType &v) const {            \
    if (v.d_ != d_) {                                                         \
      return BVector(invalid_vector_t());                                     \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = c_[i] <= v.c_[i];                                                \
    }                                                                         \
    return o;                                                                 \
  }                                                                           \
  util::StatusOr<BVector> VectorType::lessThanOrEqual(                        \
      const ParamVectorType &v) const {                                       \
    if (v.d_ != d_) {                                                         \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,                 \
                          "dimensions are not equal");                        \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = c_[i] <= v.c_[i];                                                \
    }                                                                         \
    return o;                                                                 \
  }                                                                           \
  bool VectorType::operator==(const ParamVectorType &v) const {               \
    return equals(v);                                                         \
  }                                                                           \
  bool VectorType::equals(const ParamVectorType &v, FloatType tolerance)      \
      const {                                                                 \
    return reduce(                                                            \
        [v, tolerance](bool a, DataType e, int i, DataType *A) {              \
          return a && floatEquals<FloatType>(e, v[i], tolerance);             \
        },                                                                    \
        true);                                                                \
  }                                                                           \
  util::StatusOr<BVector> VectorType::eachEquals(const ParamVectorType &v,    \
                                                 FloatType tolerance) const { \
    if (v.d_ != d_) {                                                         \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,                 \
                          "dimensions are not equal");                        \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = floatEquals<FloatType>(c_[i], v.c_[i], tolerance);               \
    }                                                                         \
    return o;                                                                 \
  }                                                                           \
  BVector VectorType::operator>=(const ParamVectorType &v) const {            \
    if (v.d_ != d_) {                                                         \
      return BVector(invalid_vector_t());                                     \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = c_[i] >= v.c_[i];                                                \
    }                                                                         \
    return o;                                                                 \
  }                                                                           \
  util::StatusOr<BVector> VectorType::greaterThanOrEqual(                     \
      const ParamVectorType &v) const {                                       \
    if (v.d_ != d_) {                                                         \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,                 \
                          "dimensions are not equal");                        \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = c_[i] >= v.c_[i];                                                \
    }                                                                         \
    return o;                                                                 \
  }                                                                           \
  BVector VectorType::operator>(const ParamVectorType &v) const {             \
    if (v.d_ != d_) {                                                         \
      return BVector(invalid_vector_t());                                     \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = c_[i] > v.c_[i];                                                 \
    }                                                                         \
    return o;                                                                 \
  }                                                                           \
  util::StatusOr<BVector> VectorType::greaterThan(const ParamVectorType &v)   \
      const {                                                                 \
    if (v.d_ != d_) {                                                         \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,                 \
                          "dimensions are not equal");                        \
    }                                                                         \
    BVector o(d_);                                                            \
    for (int i = 0; i < (int) d_; i++) {                                            \
      o[i] = c_[i] > v.c_[i];                                                 \
    }                                                                         \
    return o;                                                                 \
  }

#define VECTOR_UNARY_OP_INPLACE(VectorType, DataType, function, op)        \
  VectorType &VectorType::function() {                                     \
    return transform([](DataType e, int i, DataType *A) { return op e; }); \
  }
#define VECTOR_UNARY_OP_OUTOFPLACE(VectorType, DataType, function, op) \
  VectorType VectorType::function() const {                            \
    return map([](DataType e, int i, DataType *A) { return op e; });   \
  }
#define VECTOR_UNARY_OP_OUTOFPLACE_O(VectorType, DataType, op)       \
  VectorType VectorType::operator op() const {                       \
    return map([](DataType e, int i, DataType *A) { return op e; }); \
  }

#define VECTOR_UNARY_OP(VectorType, DataType, functionInPlace,             \
                        functionOutOfPlace, operator)                      \
  VECTOR_UNARY_OP_INPLACE(VectorType, DataType, functionInPlace, operator) \
  VECTOR_UNARY_OP_OUTOFPLACE(VectorType, DataType,                         \
                             functionOutOfPlace, operator)                 \
  VECTOR_UNARY_OP_OUTOFPLACE_O(VectorType, DataType, operator)

#define VECTOR_UNARY_FN_S(VectorType, ReturnType, DataType, function, \
                          delegate)                                   \
  VectorType &VectorType::function() {                                \
    return transform(                                                 \
        [](DataType e, int i, DataType *A) { return delegate(e); });  \
  }
#define VECTOR_UNARY_FN_O(VectorType, ReturnType, DataType, function,       \
                          delegate)                                         \
  ReturnType VectorType::function() const {                                 \
    return map([](DataType e, int i, DataType *A) { return delegate(e); }); \
  }
#define VECTOR_UNARY_FN_O_C(VectorType, ReturnType, DataType, function,   \
                            delegate)                                     \
  ReturnType VectorType::function() const {                               \
    return ReturnType(                                                    \
        map([](DataType e, int i, DataType *A) { return delegate(e); })); \
  }

#define VECTOR_UNARY_FN(VectorType, ReturnType, DataType, functionInplace, \
                        functionOutofplace, delegate)                      \
  VECTOR_UNARY_FN_S(VectorType, VectorType, DataType, functionInplace,     \
                    delegate)                                              \
  VECTOR_UNARY_FN_O(VectorType, ReturnType, DataType, functionOutofplace,  \
                    delegate)

#define VECTOR_UNARY_FN_C(VectorType, ReturnType, DataType, functionInplace, \
                          functionOutofplace, delegate)                      \
  VECTOR_UNARY_FN_S(VectorType, ReturnType, DataType, functionInplace,       \
                    delegate)                                                \
  VECTOR_UNARY_FN_O_C(VectorType, ReturnType, DataType, functionOutofplace,  \
                      delegate)

#define VECTOR_UNARY_FNS(VectorType, FloatVectorType, IntegerVectorType,      \
                         DataType)                                            \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, cos, cos_of, ::cos)  \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, sin, sin_of, ::sin)  \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, tan, tan_of, ::tan)  \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, acos, acos_of,       \
                  ::acos)                                                     \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, asin, asin_of,       \
                  ::asin)                                                     \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, atan, atan_of,       \
                  ::atan)                                                     \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, cosh, cosh_of,       \
                  ::cosh)                                                     \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, sinh, sinh_of,       \
                  ::sinh)                                                     \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, tanh, tanh_of,       \
                  ::tanh)                                                     \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, acosh, acosh_of,     \
                  ::acosh)                                                    \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, asinh, asinh_of,     \
                  ::asinh)                                                    \
  VECTOR_UNARY_FN(VectorType, FloatVectorType, DataType, atanh, atanh_of,     \
                  ::atanh)                                                    \
  VECTOR_UNARY_FN_C(VectorType, IntegerVectorType, DataType, floor, floor_of, \
                    ::floor)                                                  \
  VECTOR_UNARY_FN_C(VectorType, IntegerVectorType, DataType, ceil, ceil_of,   \
                    ::ceil)

#define VECTOR_UNARY_FNS_I(VectorType, FloatVectorType, IntegerVectorType,     \
                           DataType)                                           \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, cos, cos_of, ::cos) \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, sin, sin_of, ::sin) \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, tan, tan_of, ::tan) \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, acos, acos_of,      \
                    ::acos)                                                    \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, asin, asin_of,      \
                    ::asin)                                                    \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, atan, atan_of,      \
                    ::atan)                                                    \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, cosh, cosh_of,      \
                    ::cosh)                                                    \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, sinh, sinh_of,      \
                    ::sinh)                                                    \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, tanh, tanh_of,      \
                    ::tanh)                                                    \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, acosh, acosh_of,    \
                    ::acosh)                                                   \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, asinh, asinh_of,    \
                    ::asinh)                                                   \
  VECTOR_UNARY_FN_C(VectorType, FloatVectorType, DataType, atanh, atanh_of,    \
                    ::atanh)                                                   \
  VECTOR_UNARY_FN(VectorType, IntegerVectorType, DataType, floor, floor_of,    \
                  ::floor)                                                     \
  VECTOR_UNARY_FN(VectorType, IntegerVectorType, DataType, ceil, ceil_of,      \
                  ::ceil)

#define VECTOR_BINARY_OP_INPLACE_SCALAR(VectorType, DataType, function, op)   \
  VectorType &VectorType::function(const DataType v) {                        \
    return transform([v](DataType e, int i, DataType *A) { return e op v; }); \
  }
#define VECTOR_BINARY_OP_INPLACE_ARR(VectorType, DataType, function, op) \
  VectorType &VectorType::function(const DataType *v) {                  \
    return transform(                                                    \
        [v](DataType e, int i, DataType *A) { return e op v[i]; });      \
  }
#define VECTOR_BINARY_OP_INPLACE_VEC(VectorType, ParamVectorType, DataType,   \
                                     function, op)                            \
  util::StatusOr<VectorType> VectorType::function(const ParamVectorType &v) { \
    if (v.d_ != d_)                                                           \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,                 \
                          "dimensions must be equal");                        \
    return transform(                                                         \
        [v](DataType e, int i, DataType *A) { return e op v[i]; });           \
  }
#define VECTOR_BINARY_OP_INPLACE_O_SCALAR(VectorType, DataType, iop, op)      \
  VectorType &VectorType::operator iop(const DataType v) {                    \
    return transform([v](DataType e, int i, DataType *A) { return e op v; }); \
  }
#define VECTOR_BINARY_OP_INPLACE_O_ARR(VectorType, DataType, iop, op) \
  VectorType &VectorType::operator iop(const DataType *v) {           \
    return transform(                                                 \
        [v](DataType e, int i, DataType *A) { return e + v[i]; });    \
  }
#define VECTOR_BINARY_OP_INPLACE_O_VEC(VectorType, ParamVectorType, DataType, \
                                       iop, op)                               \
  VectorType &VectorType::operator iop(const ParamVectorType &v) {            \
    return transform(                                                         \
        [v](DataType e, int i, DataType *A) { return e + v[i]; });            \
  }
#define VECTOR_BINARY_OP_OUTOFPLACE_SCALAR(VectorType, DataType, function, op) \
  VectorType VectorType::function(const DataType v) const {                    \
    return map([v](DataType e, int i, DataType *A) { return e op v; });        \
  }
#define VECTOR_BINARY_OP_OUTOFPLACE_ARR(VectorType, DataType, function, op) \
  VectorType VectorType::function(const DataType *v) const {                \
    return map([v](DataType e, int i, DataType *A) { return e op v[i]; });  \
  }
#define VECTOR_BINARY_OP_OUTOFPLACE_VEC(                                   \
    VectorType, ReturnVectorType, ParamVectorType, DataType, function, op) \
  util::StatusOr<ReturnVectorType> VectorType::function(                   \
      const ParamVectorType &v) const {                                    \
    if (v.d_ != d_)                                                        \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,              \
                          "dimensions must be equal");                     \
    return ReturnVectorType(                                               \
        map([v](DataType e, int i, DataType *A) { return e op v[i]; }));   \
  }
#define VECTOR_BINARY_OP_OUTOFPLACE_O_SCALAR(VectorType, DataType, op)  \
  VectorType VectorType::operator op(const DataType v) const {          \
    return map([v](DataType e, int i, DataType *A) { return e op v; }); \
  }
#define VECTOR_BINARY_OP_OUTOFPLACE_O_ARR(VectorType, DataType, op)        \
  VectorType VectorType::operator op(const DataType *v) const {            \
    return map([v](DataType e, int i, DataType *A) { return e op v[i]; }); \
  }
#define VECTOR_BINARY_OP_OUTOFPLACE_O_VEC(VectorType, ReturnVectorType,      \
                                          ParamVectorType, DataType, op)     \
  ReturnVectorType VectorType::operator op(const ParamVectorType &v) const { \
    if (v.d_ != d_) return ReturnVectorType(invalid_vector_t());             \
    return ReturnVectorType(                                                 \
        map([v](DataType e, int i, DataType *A) { return e op v[i]; }));     \
  }

#define VECTOR_BINARY_OP_INPLACE(VectorType, DataType, function, op)  \
  VECTOR_BINARY_OP_INPLACE_SCALAR(VectorType, DataType, function, op) \
  VECTOR_BINARY_OP_INPLACE_ARR(VectorType, DataType, function, op)    \
  VECTOR_BINARY_OP_INPLACE_VEC(VectorType, VectorType, DataType, function, op)

#define VECTOR_BINARY_OP_INPLACE_O(VectorType, DataType, iop, op)  \
  VECTOR_BINARY_OP_INPLACE_O_SCALAR(VectorType, DataType, iop, op) \
  VECTOR_BINARY_OP_INPLACE_O_ARR(VectorType, DataType, iop, op)    \
  VECTOR_BINARY_OP_INPLACE_O_VEC(VectorType, VectorType, DataType, iop, op)

#define VECTOR_BINARY_OP_OUTOFPLACE(VectorType, DataType, function, op)  \
  VECTOR_BINARY_OP_OUTOFPLACE_SCALAR(VectorType, DataType, function, op) \
  VECTOR_BINARY_OP_OUTOFPLACE_ARR(VectorType, DataType, function, op)    \
  VECTOR_BINARY_OP_OUTOFPLACE_VEC(VectorType, VectorType, VectorType,    \
                                  DataType, function, op)

#define VECTOR_BINARY_OP_OUTOFPLACE_O(VectorType, DataType, op)         \
  VECTOR_BINARY_OP_OUTOFPLACE_O_SCALAR(VectorType, DataType, op)        \
  VECTOR_BINARY_OP_OUTOFPLACE_O_ARR(VectorType, DataType, op)           \
  VECTOR_BINARY_OP_OUTOFPLACE_O_VEC(VectorType, VectorType, VectorType, \
                                    DataType, op)

#define VECTOR_BINARY_OP(VectorType, DataType, functionInPlace,         \
                         inPlaceOperator, functionOutOfPlace,           \
                         outOfPlaceOperator)                            \
  VECTOR_BINARY_OP_INPLACE(VectorType, DataType, functionInPlace,       \
                           outOfPlaceOperator)                          \
  VECTOR_BINARY_OP_INPLACE_O(VectorType, DataType, inPlaceOperator,     \
                             outOfPlaceOperator)                        \
  VECTOR_BINARY_OP_OUTOFPLACE(VectorType, DataType, functionOutOfPlace, \
                              outOfPlaceOperator)                       \
  VECTOR_BINARY_OP_OUTOFPLACE_O(VectorType, DataType, outOfPlaceOperator)

#define VECTOR_BINARY_OP_VEC(VectorType, ReturnVectorType, ParamVectorType, \
                             DataType, functionInPlace, inPlaceOperator,    \
                             functionOutOfPlace, outOfPlaceOperator)        \
  VECTOR_BINARY_OP_INPLACE_VEC(VectorType, ParamVectorType, DataType,       \
                               functionInPlace, outOfPlaceOperator)         \
  VECTOR_BINARY_OP_INPLACE_O_VEC(VectorType, ParamVectorType, DataType,     \
                                 inPlaceOperator, outOfPlaceOperator)       \
  VECTOR_BINARY_OP_OUTOFPLACE_VEC(VectorType, ReturnVectorType,             \
                                  ParamVectorType, DataType,                \
                                  functionOutOfPlace, outOfPlaceOperator)   \
  VECTOR_BINARY_OP_OUTOFPLACE_O_VEC(VectorType, ReturnVectorType,           \
                                    ParamVectorType, DataType,              \
                                    outOfPlaceOperator)

#define VECTOR_BINARY_OP_ARITHS(VectorType, DataType)                 \
  VECTOR_BINARY_OP(VectorType, DataType, add, +=, sum, +)             \
  VECTOR_BINARY_OP(VectorType, DataType, subtract, -=, difference, -) \
  VECTOR_BINARY_OP(VectorType, DataType, multiply, *=, product, *)    \
  VECTOR_BINARY_OP(VectorType, DataType, divide, /=, quotient, /)

#define VECTOR_BINARY_OP_VEC_ARITHS(VectorType, ReturnVectorType,     \
                                    ParamVectorType, DataType)        \
  VECTOR_BINARY_OP_VEC(VectorType, ReturnVectorType, ParamVectorType, \
                       DataType, add, +=, sum, +)                     \
  VECTOR_BINARY_OP_VEC(VectorType, ReturnVectorType, ParamVectorType, \
                       DataType, subtract, -=, difference, -)         \
  VECTOR_BINARY_OP_VEC(VectorType, ReturnVectorType, ParamVectorType, \
                       DataType, multiply, *=, product, *)            \
  VECTOR_BINARY_OP_VEC(VectorType, ReturnVectorType, ParamVectorType, \
                       DataType, divide, /=, quotient, /)

#define VECTOR_BINARY_OP_NOIOP(VectorType, DataType, functionInPlace,   \
                               functionOutOfPlace, outOfPlaceOperator)  \
  VECTOR_BINARY_OP_INPLACE(VectorType, DataType, functionInPlace)       \
  VECTOR_BINARY_OP_OUTOFPLACE(VectorType, DataType, functionOutOfPlace) \
  VECTOR_BINARY_OP_OUTOFPLACE_O(VectorType, DataType, outOfPlaceOperator)

#define VECTOR_BINARY_OP_LOGICS(VectorType)                          \
  VECTOR_BINARY_OP_NOIOP(VectorType, bool, bor, disjunction, ||)     \
  VECTOR_BINARY_OP_NOIOP(VectorType, bool, band, conjunction, &&)    \
  VECTOR_BINARY_OP_INPLACE_O_VEC(VectorType, VectorType, bool, +=)   \
  VECTOR_BINARY_OP_OUTOFPLACE_O_VEC(VectorType, VectorType, bool, +) \
  VECTOR_BINARY_OP_INPLACE_O_VEC(VectorType, VectorType, bool, *=)   \
  VECTOR_BINARY_OP_OUTOFPLACE_O_VEC(VectorType, VectorType, bool, *)

#define VECTOR_BINARY_FN_INPLACE_SCALAR(VectorType, DataType, f, delegate) \
  VectorType &VectorType::f(const DataType v) {                            \
    return transform(                                                      \
        [v](DataType e, int i, DataType *A) { return delegate(e, v); });   \
  }
#define VECTOR_BINARY_FN_INPLACE_ARR(VectorType, DataType, f, delegate)     \
  VectorType &VectorType::f(const DataType *v) {                            \
    return transform(                                                       \
        [v](DataType e, int i, DataType *A) { return delegate(e, v[i]); }); \
  }
#define VECTOR_BINARY_FN_INPLACE_VEC(VectorType, ParamVectorType, DataType, f, \
                                     delegate)                                 \
  util::StatusOr<VectorType> VectorType::f(const ParamVectorType &v) {         \
    if (v.d_ != d_)                                                            \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,                  \
                          "dimensions must be equal");                         \
    return transform(                                                          \
        [v](DataType e, int i, DataType *A) { return delegate(e, v[i]); });    \
  }
#define VECTOR_BINARY_FN_INPLACE_O_SCALAR(VectorType, DataType, f, delegate) \
  VectorType &VectorType::operator f(const DataType v) {                     \
    return transform(                                                        \
        [v](DataType e, int i, DataType *A) { return delegate(e, v); });     \
  }
#define VECTOR_BINARY_FN_INPLACE_O_ARR(VectorType, DataType, f, delegate)   \
  VectorType &VectorType::operator f(const DataType *v) {                   \
    return transform(                                                       \
        [v](DataType e, int i, DataType *A) { return delegate(e, v[i]); }); \
  }
#define VECTOR_BINARY_FN_INPLACE_O_VEC(VectorType, ParamVectorType, DataType, \
                                       f, delegate)                           \
  VectorType &VectorType::operator f(const ParamVectorType &v) {              \
    return transform(                                                         \
        [v](DataType e, int i, DataType *A) { return delegate(e, v[i]); });   \
  }
#define VECTOR_BINARY_FN_OUTOFPLACE_SCALAR(VectorType, DataType, f, delegate) \
  VectorType VectorType::f(const DataType v) const {                          \
    return map(                                                               \
        [v](DataType e, int i, DataType *A) { return delegate(e, v); });      \
  }
#define VECTOR_BINARY_FN_OUTOFPLACE_ARR(VectorType, DataType, f, delegate)  \
  VectorType VectorType::f(const DataType *v) const {                       \
    return map(                                                             \
        [v](DataType e, int i, DataType *A) { return delegate(e, v[i]); }); \
  }
#define VECTOR_BINARY_FN_OUTOFPLACE_VEC(                                     \
    VectorType, ReturnVectorType, ParamVectorType, DataType, f, delegate)    \
  util::StatusOr<ReturnVectorType> VectorType::f(const ParamVectorType &v)   \
      const {                                                                \
    if (v.d_ != d_)                                                          \
      return util::Status(util::StatusCode::INVALID_ARGUMENT,                \
                          "dimensions must be equal");                       \
    return ReturnVectorType(map(                                             \
        [v](DataType e, int i, DataType *A) { return delegate(e, v[i]); })); \
  }
#define VECTOR_BINARY_FN_OUTOFPLACE_O_SCALAR(VectorType, DataType, op,   \
                                             delegate)                   \
  VectorType VectorType::operator op(const DataType v) const {           \
    return map(                                                          \
        [v](DataType e, int i, DataType *A) { return delegate(e, v); }); \
  }
#define VECTOR_BINARY_FN_OUTOFPLACE_O_ARR(VectorType, DataType, op, delegate) \
  VectorType VectorType::operator op(const DataType *v) const {               \
    return map(                                                               \
        [v](DataType e, int i, DataType *A) { return delegate(e, v[i]); });   \
  }
#define VECTOR_BINARY_FN_OUTOFPLACE_O_VEC(                                   \
    VectorType, ReturnVectorType, ParamVectorType, DataType, op, delegate)   \
  ReturnVectorType VectorType::operator op(const ParamVectorType &v) const { \
    return ReturnVectorType(map(                                             \
        [v](DataType e, int i, DataType *A) { return delegate(e, v[i]); })); \
  }

#define VECTOR_BINARY_FN_INPLACE(VectorType, DataType, f, delegate)  \
  VECTOR_BINARY_FN_INPLACE_SCALAR(VectorType, DataType, f, delegate) \
  VECTOR_BINARY_FN_INPLACE_ARR(VectorType, DataType, f, delegate)    \
  VECTOR_BINARY_FN_INPLACE_VEC(VectorType, VectorType, DataType, f, delegate)

#define VECTOR_BINARY_FN_INPLACE_O(VectorType, DataType, f, delegate)  \
  VECTOR_BINARY_FN_INPLACE_O_SCALAR(VectorType, DataType, f, delegate) \
  VECTOR_BINARY_FN_INPLACE_O_ARR(VectorType, DataType, f, delegate)    \
  VECTOR_BINARY_FN_INPLACE_O_VEC(VectorType, VectorType, DataType, f, delegate)

#define VECTOR_BINARY_FN_OUTOFPLACE(VectorType, DataType, f, delegate)  \
  VECTOR_BINARY_FN_OUTOFPLACE_SCALAR(VectorType, DataType, f, delegate) \
  VECTOR_BINARY_FN_OUTOFPLACE_ARR(VectorType, DataType, f, delegate)    \
  VECTOR_BINARY_FN_OUTOFPLACE_VEC(VectorType, VectorType, VectorType,   \
                                  DataType, f, delegate)

#define VECTOR_BINARY_FN_OUTOFPLACE_O(VectorType, DataType, op, delegate)  \
  VECTOR_BINARY_FN_OUTOFPLACE_O_SCALAR(VectorType, DataType, op, delegate) \
  VECTOR_BINARY_FN_OUTOFPLACE_O_ARR(VectorType, DataType, op, delegate)    \
  VECTOR_BINARY_FN_OUTOFPLACE_O_VEC(VectorType, VectorType, VectorType,    \
                                    DataType, op, delegate)

#define VECTOR_BINARY_FN(VectorType, DataType, functionInPlace,               \
                         inPlaceOperator, functionOutOfPlace,                 \
                         outOfPlaceOperator, delegate)                        \
  VECTOR_BINARY_FN_INPLACE(VectorType, DataType, functionInPlace, delegate)   \
  VECTOR_BINARY_FN_INPLACE_O(VectorType, DataType, inPlaceOperator, delegate) \
  VECTOR_BINARY_FN_OUTOFPLACE(VectorType, DataType, functionOutOfPlace,       \
                              delegate)                                       \
  VECTOR_BINARY_FN_OUTOFPLACE_O(VectorType, DataType, outOfPlaceOperator,     \
                                delegate)

#define VECTOR_BINARY_FN_VEC(VectorType, ReturnVectorType, ParamVectorType,    \
                             DataType, functionInPlace, inPlaceOperator,       \
                             functionOutOfPlace, outOfPlaceOperator, delegate) \
  VECTOR_BINARY_FN_INPLACE_VEC(VectorType, ParamVectorType, DataType,          \
                               functionInPlace, delegate)                      \
  VECTOR_BINARY_FN_INPLACE_O_VEC(VectorType, ParamVectorType, DataType,        \
                                 inPlaceOperator, delegate)                    \
  VECTOR_BINARY_FN_OUTOFPLACE_VEC(VectorType, ReturnVectorType,                \
                                  ParamVectorType, DataType,                   \
                                  functionOutOfPlace, delegate)                \
  VECTOR_BINARY_FN_OUTOFPLACE_O_VEC(VectorType, ReturnVectorType,              \
                                    ParamVectorType, DataType,                 \
                                    outOfPlaceOperator, delegate)

#define VECTOR_BINARY_MISC(VectorType, ReturnVectorType, ParamVectorType,      \
                           DataType, FloatType)                                \
  util::StatusOr<DataType> VectorType::dot(const ParamVectorType &v) const {   \
    return reduce([v](DataType a, DataType e, int i,                           \
                      DataType *A) -> DataType { return a + e * v[i]; },       \
                  0.0);                                                        \
  }                                                                            \
  FloatType VectorType::angleTo(const ParamVectorType &a) const {              \
    return ::acos(dot(a).ValueOrDie() / (length() * a.length()));              \
  }                                                                            \
  util::StatusOr<ReturnVectorType> VectorType::min(const ParamVectorType &v)   \
      const {                                                                  \
    return map([v](DataType e, int i, DataType *A) { return fmin(e, v[i]); }); \
  }                                                                            \
  util::StatusOr<ReturnVectorType> VectorType::max(const ParamVectorType &v)   \
      const {                                                                  \
    return map([v](DataType e, int i, DataType *A) { return fmax(e, v[i]); }); \
  }

#define VECTOR_NORM(VectorType, FloatVectorType, DataType, FloatType)          \
  FloatType VectorType::length2() const { return dot(*this).ValueOrDie(); }    \
  FloatType VectorType::length() const {                                       \
    return sqrt(dot(*this).ValueOrDie());                                      \
  }                                                                            \
  DataType VectorType::manhattan() const {                                     \
    return reduce(                                                             \
        [](DataType a, DataType e, int i, DataType *A) { return a + e; }, 0);  \
  }                                                                            \
  FloatType VectorType::pnorm(int p) const {                                   \
    return pow(reduce([p](DataType a, DataType e, int i,                       \
                          DataType *A) { return a + pow(e, p); },              \
                      0),                                                      \
               1. / p);                                                        \
  }                                                                            \
  FloatType VectorType::pnormn(int p, int n) const {                           \
    return pow(reduce([p, n](DataType a, DataType e, int i,                    \
                             DataType *A) { return a + pow(e, p); },           \
                      0),                                                      \
               1. * n / p);                                                    \
  }                                                                            \
  VectorType &VectorType::normalize() { return divide(length()); }             \
  FloatVectorType VectorType::unit() const {                                   \
    return FloatVectorType(quotient(length()));                                \
  }                                                                            \
  DataType VectorType::min() const {                                           \
    return reduce(                                                             \
        [](DataType a, DataType e, int i, DataType *A) { return fmin(a, e); }, \
        c_[0]);                                                                \
  }                                                                            \
  DataType VectorType::max() const {                                           \
    return reduce(                                                             \
        [](DataType a, DataType e, int i, DataType *A) { return fmax(a, e); }, \
        c_[0]);                                                                \
  }

VECTOR_CONVERSION_CONSTRUCTOR(BVector, IVector)
VECTOR_CONVERSION_CONSTRUCTOR(BVector, FVector)
VECTOR_CONVERSION_CONSTRUCTOR(BVector, DVector)

VECTOR_CONVERSION_CONSTRUCTOR(IVector, FVector)
VECTOR_CONVERSION_CONSTRUCTOR(IVector, DVector)

VECTOR_CONVERSION_CONSTRUCTOR(FVector, IVector)
VECTOR_CONVERSION_CONSTRUCTOR(FVector, DVector)

VECTOR_CONVERSION_CONSTRUCTOR(DVector, IVector)
VECTOR_CONVERSION_CONSTRUCTOR(DVector, FVector)

VECTOR_COMP(IVector, IVector, int, float)
// VECTOR_COMP(IVector, FVector, int, float)
// VECTOR_COMP(IVector, DVector, int, double)
// VECTOR_COMP(FVector, IVector, float, float)
VECTOR_COMP(FVector, FVector, float, float)
// VECTOR_COMP(FVector, DVector, float, double)
// VECTOR_COMP(DVector, IVector, double, double)
// VECTOR_COMP(DVector, FVector, double, double)
VECTOR_COMP(DVector, DVector, double, double)

VECTOR_UNARY_OP(BVector, bool, bnot, negation, !)
VECTOR_UNARY_OP_OUTOFPLACE_O(BVector, bool, -)

VECTOR_UNARY_OP(IVector, int, negate, negation, -)

VECTOR_UNARY_OP(FVector, float, negate, negation, -)

VECTOR_UNARY_OP(DVector, double, negate, negation, -)

VECTOR_UNARY_FNS_I(IVector, FVector, IVector, int)

VECTOR_UNARY_FNS(FVector, FVector, IVector, float)

VECTOR_UNARY_FNS(DVector, DVector, IVector, double)

VECTOR_BINARY_OP_ARITHS(IVector, int)
VECTOR_BINARY_OP_VEC_ARITHS(IVector, FVector, FVector, int)
VECTOR_BINARY_OP_VEC_ARITHS(IVector, DVector, DVector, int)

VECTOR_BINARY_OP_ARITHS(FVector, float)
VECTOR_BINARY_OP_VEC_ARITHS(FVector, FVector, IVector, float)
VECTOR_BINARY_OP_VEC_ARITHS(FVector, DVector, DVector, float)

VECTOR_BINARY_OP_ARITHS(DVector, double)
VECTOR_BINARY_OP_VEC_ARITHS(DVector, DVector, IVector, double)
VECTOR_BINARY_OP_VEC_ARITHS(DVector, DVector, FVector, double)

VECTOR_BINARY_OP(IVector, int, mod, %=, modulus, %)
VECTOR_BINARY_FN_VEC(IVector, FVector, FVector, int, mod, %=, modulus, %, fmod)
VECTOR_BINARY_FN_VEC(IVector, DVector, DVector, int, mod, %=, modulus, %, fmod)

VECTOR_BINARY_FN(FVector, float, mod, %=, modulus, %, fmod)
VECTOR_BINARY_FN_VEC(FVector, FVector, IVector, float, mod, %=, modulus, %,
                     fmod)
VECTOR_BINARY_FN_VEC(FVector, DVector, DVector, float, mod, %=, modulus, %,
                     fmod)

VECTOR_BINARY_FN(DVector, double, mod, %=, modulus, %, fmod)
VECTOR_BINARY_FN_VEC(DVector, DVector, IVector, double, mod, %=, modulus, %,
                     fmod)
VECTOR_BINARY_FN_VEC(DVector, DVector, FVector, double, mod, %=, modulus, %,
                     fmod)

VECTOR_BINARY_MISC(IVector, IVector, IVector, int, float)

VECTOR_BINARY_MISC(FVector, FVector, FVector, float, float)

VECTOR_BINARY_MISC(DVector, DVector, DVector, double, double)

VECTOR_NORM(IVector, FVector, int, float)

VECTOR_NORM(FVector, FVector, float, float)

VECTOR_NORM(DVector, DVector, double, double)

/*
util::StatusOr<FVector> FVector::crossEquals(const FVector *vs) {
  if (d == 2) {
    var x = x;
    x = y;
    y = -x;
    return this;
  }
  var rows = _.times(d, i => i ? (i == 1 ? this : (_.isArray(vs) ? vs[i - 2] :
vs)).toArray() : _.times(d, () => 0)); for (var i = 0; i < d; i++) { this[i] =
cofactor(rows, 0, i);
  }
  return this;
}
util::StatusOr<FVector> FVector::cross(const FVector *vs) {
  if (d == 2) {
    return new FVector(2, y, -x);
  }
  var rows = _.times(d, i => i ? (i == 1 ? this : (_.isArray(vs) ? vs[i - 2] :
vs)).toArray() : _.times(d, () => 0)); return map((e, i) => cofactor(rows, 0,
i));
}
//*/

float *FVector::toAngles() {
  float *squares = new float[d_];
  float *sums = new float[d_];
  float *angles = new float[d_ - 1];
  squares[d_ - 1] = c_[d_ - 1] * c_[d_ - 1];
  sums[d_ - 1] = squares[d_ - 1];
  for (int i = d_ - 2; i >= 0; i--) {
    squares[i] = c_[i] * c_[i];
    sums[i] = sums[i + 1] + squares[i];
    angles[i] = c_[i] / sqrt(sums[i]);
    if (i == (int) d_ - 2 && c_[d_ - 1] < 0) {
      angles[i] = 2 * M_PI - angles[i];
    }
  }
  return angles;
}

FVector FVector::fromAngles(int d, float *as) {
  FVector v(d);
  float *sins = new float[d - 1], product = 1;
  for (int i = 0; i < d; i++) {
    v[i] = product;
    if (i < d - 1) {
      v[i] = v[i] * ::cos(as[i]);
      product *= sins[i] = ::sin(as[i]);
    }
  }
  return v;
}
FVector FVector::fromAngles(int d, float m, float *as) {
  FVector v(d);
  float *sins = new float[d - 1], product = m;
  for (int i = 0; i < d; i++) {
    v[i] = product;
    if (i < d - 1) {
      v[i] = v[i] * ::cos(as[i]);
      product *= sins[i] = ::sin(as[i]);
    }
  }
  return v;
}
FVector FVector::randomDirection(int d) {
  float *as = new float[d - 1];
  return FVector::fromAngles(d, rand() * 2 * M_PI, as);
}
FVector FVector::lerp(const FVector &v, const FVector &w, float factor) {
  return (w - v) * factor + v;
}

}  // namespace vector
}  // namespace math
