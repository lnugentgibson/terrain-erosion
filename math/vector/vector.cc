#include "math/vector/vector.h"

#include <cmath>
#include <sstream>

#include "math/common.h"
#include "util/status.h"

namespace math {
namespace vector {

IVector::IVector(const IVector &v) : c(new int[v.d]), d(v.d) {
  for (int i = 0; i < d; i++) {
    c[i] = v[d];
  }
}
IVector::IVector(IVector &&v) : c(v.c), d(v.d) { v.c = 0; }

IVector::IVector(const Vector &v) : c(new int[v.d]), d(v.d) {
  for (int i = 0; i < d; i++) {
    c[i] = (int)v[d];
  }
}
IVector::IVector(Vector &&v) : c(new int[v.d]), d(v.d) {
  for (int i = 0; i < d; i++) {
    c[i] = (int)v[d];
  }
  delete[] v.c;
  v.c = 0;
}

IVector &IVector::operator=(const IVector &v) {
  if (v.d == d) {
    for (int i = 0; i < d; i++) {
      c[i] = v[d];
    }
  }
  return *this;
}
IVector &IVector::operator=(IVector &&v) {
  if (v.d == d) {
    delete[] c;
    c = v.c;
    v.c = 0;
  }
  return *this;
}
IVector &IVector::operator=(const Vector &v) {
  if (v.d == d) {
    for (int i = 0; i < d; i++) {
      c[i] = (int)v[d];
    }
  }
  return *this;
}
IVector &IVector::operator=(Vector &&v) {
  if (v.d == d) {
    for (int i = 0; i < d; i++) {
      c[i] = (int)v[d];
    }
    delete[] v.c;
    v.c = 0;
  }
  return *this;
}

Vector::Vector(const IVector &v) : c(new float[v.d]), d(v.d) {
  for (int i = 0; i < d; i++) {
    c[i] = (float)v[i];
  }
}
/*
Vector::Vector(IVector &&v) : c(new float[v.d]), d(v.d) {
  for (int i = 0; i < d; i++) {
    c[i] = (float)v[i];
  }
  delete[] v.c;
  v.c = 0;
}
*/

Vector::Vector(const Vector &v) : c(new float[v.d]), d(v.d) {
  for (int i = 0; i < d; i++) {
    c[i] = v[i];
  }
}

Vector::Vector(Vector &&v) : c(v.c), d(v.d) { v.c = 0; }
Vector::Vector(const Vector &&v) : c(new float[v.d]), d(v.d) {
  for (int i = 0; i < d; i++) {
    c[i] = v[i];
  }
}

Vector &Vector::operator=(const IVector &v) {
  if (v.d == d) {
    for (int i = 0; i < d; i++) {
      c[i] = (float)v[i];
    }
  }
  return *this;
}
/*
Vector &Vector::operator=(IVector &&v) {
  if (v.d == d) {
    for (int i = 0; i < d; i++) {
      c[i] = (float)v[i];
    }
    delete[] v.c;
    v.c = 0;
  }
  return *this;
}
*/
Vector &Vector::operator=(const Vector &v) {
  if (v.d == d) {
    for (int i = 0; i < d; i++) {
      c[i] = v[d];
    }
  }
  return *this;
}
Vector &Vector::operator=(Vector &&v) {
  if (v.d == d) {
    if (c != 0)
      delete[] c;
    c = v.c;
    v.c = 0;
  }
  return *this;
}
Vector &Vector::operator=(const Vector &&v) {
  if (v.d == d) {
    for (int i = 0; i < d; i++) {
      c[i] = v[d];
    }
  }
  return *this;
}

Vector &Vector::negate() {
  return transform([](float e, int i, float *A) { return -e; });
}
Vector Vector::negative() const {
  return map([](float e, int i, float *A) { return -e; });
}

#define VECTOR_UNARY_FN_S(f, fn)                                               \
  Vector &Vector::f() {                                                        \
    return transform([](float e, int i, float *A) { return fn(e); });          \
  }
#define VECTOR_UNARY_FN_O(f, r, fn)                                            \
  r Vector::f() const {                                                        \
    return map([](float e, int i, float *A) { return fn(e); });                \
  }

#define VECTOR_UNARY_FN(fs, fo, r, fn)                                         \
  VECTOR_UNARY_FN_S(fs, fn)                                                    \
  VECTOR_UNARY_FN_O(fo, r, fn)

VECTOR_UNARY_FN(cos, cos, Vector, ::cos)
VECTOR_UNARY_FN(sin, sin, Vector, ::sin)
VECTOR_UNARY_FN(tan, tan, Vector, ::tan)
VECTOR_UNARY_FN(acos, acos, Vector, ::acos)
VECTOR_UNARY_FN(asin, asin, Vector, ::asin)
VECTOR_UNARY_FN(atan, atan, Vector, ::atan)
VECTOR_UNARY_FN(cosh, cosh, Vector, ::cosh)
VECTOR_UNARY_FN(sinh, sinh, Vector, ::sinh)
VECTOR_UNARY_FN(tanh, tanh, Vector, ::tanh)
VECTOR_UNARY_FN(acosh, acosh, Vector, ::acosh)
VECTOR_UNARY_FN(asinh, asinh, Vector, ::asinh)
VECTOR_UNARY_FN(atanh, atanh, Vector, ::atanh)
VECTOR_UNARY_FN(floor, floor, IVector, ::floor)
VECTOR_UNARY_FN(ceil, ceil, IVector, ::ceil)

#define VECTOR_BINARY_OP_S_SCALAR(f, op)                                       \
  Vector &Vector::f(const float v) {                                           \
    return transform([v](float e, int i, float *A) { return e op v; });        \
  }
#define VECTOR_BINARY_OP_S_ARR(f, op)                                          \
  Vector &Vector::f(const float *v) {                                          \
    return transform([v](float e, int i, float *A) { return e op v[i]; });     \
  }
#define VECTOR_BINARY_OP_S_VEC(f, op)                                          \
  StatusOr<Vector> Vector::f(const Vector &v) {                                \
    if (v.d != d)                                                              \
      return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal"); \
    return transform([v](float e, int i, float *A) { return e op v[i]; });     \
  }
#define VECTOR_BINARY_OP_SO_SCALAR(f, op)                                      \
  Vector &Vector::operator f(const float v) {                                  \
    return transform([v](float e, int i, float *A) { return e op v; });        \
  }
#define VECTOR_BINARY_OP_SO_ARR(f, op)                                         \
  Vector &Vector::operator f(const float *v) {                                 \
    return transform([v](float e, int i, float *A) { return e + v[i]; });      \
  }
#define VECTOR_BINARY_OP_SO_VEC(f, op)                                         \
  Vector &Vector::operator f(const Vector &v) {                                \
    return transform([v](float e, int i, float *A) { return e + v[i]; });      \
  }
#define VECTOR_BINARY_OP_O_SCALAR(f, op)                                       \
  Vector Vector::f(const float v) const {                                      \
    return map([v](float e, int i, float *A) { return e op v; });              \
  }
#define VECTOR_BINARY_OP_O_ARR(f, op)                                          \
  Vector Vector::f(const float *v) const {                                     \
    return map([v](float e, int i, float *A) { return e op v[i]; });           \
  }
#define VECTOR_BINARY_OP_O_VEC(f, op)                                          \
  StatusOr<Vector> Vector::f(const Vector &v) const {                          \
    if (v.d != d)                                                              \
      return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal"); \
    return map([v](float e, int i, float *A) { return e op v[i]; });           \
  }
#define VECTOR_BINARY_OP_OO_SCALAR(op)                                         \
  Vector Vector::operator op(const float v) const {                            \
    return map([v](float e, int i, float *A) { return e op v; });              \
  }
#define VECTOR_BINARY_OP_OO_ARR(op)                                            \
  Vector Vector::operator op(const float *v) const {                           \
    return map([v](float e, int i, float *A) { return e op v[i]; });           \
  }
#define VECTOR_BINARY_OP_OO_VEC(op)                                            \
  Vector Vector::operator op(const Vector &v) const {                          \
    return map([v](float e, int i, float *A) { return e op v[i]; });           \
  }

#define VECTOR_BINARY_OP_S(f, op)                                              \
  VECTOR_BINARY_OP_S_SCALAR(f, op)                                             \
  VECTOR_BINARY_OP_S_ARR(f, op)                                                \
  VECTOR_BINARY_OP_S_VEC(f, op)

#define VECTOR_BINARY_OP_SO(f, op)                                             \
  VECTOR_BINARY_OP_SO_SCALAR(f, op)                                            \
  VECTOR_BINARY_OP_SO_ARR(f, op)                                               \
  VECTOR_BINARY_OP_SO_VEC(f, op)

#define VECTOR_BINARY_OP_O(f, op)                                              \
  VECTOR_BINARY_OP_O_SCALAR(f, op)                                             \
  VECTOR_BINARY_OP_O_ARR(f, op)                                                \
  VECTOR_BINARY_OP_O_VEC(f, op)

#define VECTOR_BINARY_OP_OO(op)                                                \
  VECTOR_BINARY_OP_OO_SCALAR(op)                                               \
  VECTOR_BINARY_OP_OO_ARR(op)                                                  \
  VECTOR_BINARY_OP_OO_VEC(op)

#define VECTOR_BINARY_OP(fs, fso, fo, op)                                      \
  VECTOR_BINARY_OP_S(fs, op)                                                   \
  VECTOR_BINARY_OP_SO(fso, op)                                                 \
  VECTOR_BINARY_OP_O(fo, op)                                                   \
  VECTOR_BINARY_OP_OO(op)

VECTOR_BINARY_OP(add, +=, sum, +)
VECTOR_BINARY_OP(subtract, -=, difference, -)
VECTOR_BINARY_OP(multiply, *=, product, *)
VECTOR_BINARY_OP(divide, /=, quotient, /)

#define VECTOR_BINARY_FN_S_SCALAR(f, fn)                                       \
  Vector &Vector::f(const float v) {                                           \
    return transform([v](float e, int i, float *A) { return fn(e, v); });      \
  }
#define VECTOR_BINARY_FN_S_ARR(f, fn)                                          \
  Vector &Vector::f(const float *v) {                                          \
    return transform([v](float e, int i, float *A) { return fn(e, v[i]); });   \
  }
#define VECTOR_BINARY_FN_S_VEC(f, fn)                                          \
  StatusOr<Vector> Vector::f(const Vector &v) {                                \
    if (v.d != d)                                                              \
      return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal"); \
    return transform([v](float e, int i, float *A) { return fn(e, v[i]); });   \
  }
#define VECTOR_BINARY_FN_SO_SCALAR(f, fn)                                      \
  Vector &Vector::operator f(const float v) {                                  \
    return transform([v](float e, int i, float *A) { return fn(e, v); });      \
  }
#define VECTOR_BINARY_FN_SO_ARR(f, fn)                                         \
  Vector &Vector::operator f(const float *v) {                                 \
    return transform([v](float e, int i, float *A) { return fn(e, v[i]); });   \
  }
#define VECTOR_BINARY_FN_SO_VEC(f, fn)                                         \
  Vector &Vector::operator f(const Vector &v) {                                \
    return transform([v](float e, int i, float *A) { return fn(e, v[i]); });   \
  }
#define VECTOR_BINARY_FN_O_SCALAR(f, fn)                                       \
  Vector Vector::f(const float v) const {                                      \
    return map([v](float e, int i, float *A) { return fn(e, v); });            \
  }
#define VECTOR_BINARY_FN_O_ARR(f, fn)                                          \
  Vector Vector::f(const float *v) const {                                     \
    return map([v](float e, int i, float *A) { return fn(e, v[i]); });         \
  }
#define VECTOR_BINARY_FN_O_VEC(f, fn)                                          \
  StatusOr<Vector> Vector::f(const Vector &v) const {                          \
    if (v.d != d)                                                              \
      return util::Status(util::INVALID_ARGUMENT, "dimensions must be equal"); \
    return map([v](float e, int i, float *A) { return fn(e, v[i]); });         \
  }
#define VECTOR_BINARY_FN_OO_SCALAR(op, fn)                                     \
  Vector Vector::operator op(const float v) const {                            \
    return map([v](float e, int i, float *A) { return fn(e, v); });            \
  }
#define VECTOR_BINARY_FN_OO_ARR(op, fn)                                        \
  Vector Vector::operator op(const float *v) const {                           \
    return map([v](float e, int i, float *A) { return fn(e, v[i]); });         \
  }
#define VECTOR_BINARY_FN_OO_VEC(op, fn)                                        \
  Vector Vector::operator op(const Vector &v) const {                          \
    return map([v](float e, int i, float *A) { return fn(e, v[i]); });         \
  }

#define VECTOR_BINARY_FN_S(f, fn)                                              \
  VECTOR_BINARY_FN_S_SCALAR(f, fn)                                             \
  VECTOR_BINARY_FN_S_ARR(f, fn)                                                \
  VECTOR_BINARY_FN_S_VEC(f, fn)

#define VECTOR_BINARY_FN_SO(f, fn)                                             \
  VECTOR_BINARY_FN_SO_SCALAR(f, fn)                                            \
  VECTOR_BINARY_FN_SO_ARR(f, fn)                                               \
  VECTOR_BINARY_FN_SO_VEC(f, fn)

#define VECTOR_BINARY_FN_O(f, fn)                                              \
  VECTOR_BINARY_FN_O_SCALAR(f, fn)                                             \
  VECTOR_BINARY_FN_O_ARR(f, fn)                                                \
  VECTOR_BINARY_FN_O_VEC(f, fn)

#define VECTOR_BINARY_FN_OO(op, fn)                                            \
  VECTOR_BINARY_FN_OO_SCALAR(op, fn)                                           \
  VECTOR_BINARY_FN_OO_ARR(op, fn)                                              \
  VECTOR_BINARY_FN_OO_VEC(op, fn)

#define VECTOR_BINARY_FN(fs, fso, fo, op, fn)                                  \
  VECTOR_BINARY_FN_S(fs, fn)                                                   \
  VECTOR_BINARY_FN_SO(fso, fn)                                                 \
  VECTOR_BINARY_FN_O(fo, fn)                                                   \
  VECTOR_BINARY_FN_OO(op, fn)

VECTOR_BINARY_FN(mod, %=, modulus, %, fmod)

bool Vector::operator==(const Vector &v) const { return equals(v); }
bool Vector::equals(const Vector &v, float tolerance) const {
  return reduce(
      [v, tolerance](bool a, float e, int i, float *A) {
        return a && floatEquals(e, v[i], tolerance);
      },
      true);
}
StatusOr<float> Vector::dot(const Vector &v) const {
  return reduce(
      [v](float a, float e, int i, float *A) -> float { return a + e * v[i]; },
      0.0);
}
/*
StatusOr<Vector> Vector::crossEquals(const Vector *vs) {
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
StatusOr<Vector> Vector::cross(const Vector *vs) {
  if (d == 2) {
    return new Vector(2, y, -x);
  }
  var rows = _.times(d, i => i ? (i == 1 ? this : (_.isArray(vs) ? vs[i - 2] :
vs)).toArray() : _.times(d, () => 0)); return map((e, i) => cofactor(rows, 0,
i));
}
//*/
float Vector::length2() const { return *dot(*this); }
float Vector::length() const { return sqrt(*dot(*this)); }
Vector &Vector::normalize() { return divide(length()); }
Vector Vector::unit() const { return quotient(length()); }
StatusOr<Vector> Vector::min(const Vector &v) const {
  return map([v](float e, int i, float *A) { return fmin(e, v[i]); });
}
StatusOr<Vector> Vector::max(const Vector &v) const {
  return map([v](float e, int i, float *A) { return fmax(e, v[i]); });
}
float Vector::min() const {
  return reduce([](float a, float e, int i, float *A) { return fmin(a, e); },
                c[0]);
}
float Vector::max() const {
  return reduce([](float a, float e, int i, float *A) { return fmax(a, e); },
                c[0]);
}
/*
float *Vector::toAngles() {
  return {
    theta: atan2(z, x),
    phi: asin(y / length())
  };
}
//*/
float Vector::angleTo(const Vector &a) const {
  return ::acos(*dot(a) / (length() * a.length()));
}
float *Vector::toArray() const {
  float *A = new float[d];
  std::copy(c, c + d, A);
  return A;
}
Vector Vector::clone() const { return Vector(d, toArray()); }
void Vector::set(float *v) { std::copy(v, v + d, c); }
std::string Vector::toStr() const {
  std::stringstream ss;
  ss << "Vector" << d << '[';
  ss << c[0];
  for (int i = 1; i < d; i++) {
    ss << ',' << c[i];
  }
  ss << ']';
  return ss.str();
}

/*
Vector Vector::fromAnglesfloat *as) {
  return new Vector(3, cos(theta) * cos(phi), sin(phi), sin(theta) * cos(phi));
};
Vector.randomDirection = function() {
  return Vector.fromAngles(rand() * TAU, asin(rand() * 2 - 1));
};
Vector.lerp = function(a, b, fraction) {
  return b.subtract(a).multiply(fraction).add(a);
};
//*/

} // namespace vector
} // namespace math