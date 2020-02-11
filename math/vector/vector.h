#ifndef MATH_VECTOR_VECTOR_H
#define MATH_VECTOR_VECTOR_H

#include <algorithm>
#include <string>

#include "util/status.h"
#include "util/statusor.h"

using util::Status;
using util::StatusOr;

namespace math {
namespace vector {

class Matrix;
class BVector;
class IVector;
class Vector;

class BVector {
  bool *c;

public:
  const int d;
  BVector(int _d) : c(new bool[_d]), d(_d) {}
  BVector(int _d, bool *_c) : c(_c), d(_d) {}
  BVector(bool x, bool y) : c(new bool[2]), d(2) {
    c[0] = x;
    c[1] = y;
  }
  BVector(bool x, bool y, bool z) : c(new bool[3]), d(3) {
    c[0] = x;
    c[1] = y;
    c[2] = z;
  }
  BVector(bool x, bool y, bool z, bool w) : c(new bool[4]), d(4) {
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = w;
  }
  bool operator[](int i) const { return c[i]; }
  bool &operator[](int i) { return c[i]; }
};

#define VECTOR_UNARY_FN_S_SIG(f, r) r &f();
#define VECTOR_UNARY_FN_O_SIG(f, r) r f() const;

#define VECTOR_UNARY_FN_SIG(v, f, r)                                           \
  VECTOR_UNARY_FN_S_SIG(f, v)                                                  \
  VECTOR_UNARY_FN_O_SIG(f, r)

class IVector {
  int *c;

public:
  const int d;
  IVector(int _d) : c(new int[_d]), d(_d) {}
  IVector(int _d, int *_c) : c(_c), d(_d) {}
  IVector(int x, int y) : c(new int[2]), d(2) {
    c[0] = x;
    c[1] = y;
  }
  IVector(int x, int y, int z) : c(new int[3]), d(3) {
    c[0] = x;
    c[1] = y;
    c[2] = z;
  }
  IVector(int x, int y, int z, int w) : c(new int[4]), d(4) {
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = w;
  }
  IVector(const IVector &v);
  IVector(IVector &&v);
  IVector(const Vector &v);
  IVector(Vector &&v);
  IVector &operator=(const IVector &v);
  IVector &operator=(IVector &&v);
  IVector &operator=(const Vector &v);
  IVector &operator=(Vector &&v);
  ~IVector() {
    if (c)
      delete[] c;
  }
  inline bool valid() const { return c != 0; }
  inline int operator[](size_t i) const { return c[i]; }
  inline int &operator[](size_t i) { return c[i]; }

  friend Vector;
};

class Vector {
  float *c;

public:
  const int d;
  Vector(int _d) : c(new float[_d]), d(_d) {}
  Vector(int _d, float *_c) : c(_c), d(_d) {}
  Vector(float x, float y) : c(new float[2]), d(2) {
    c[0] = x;
    c[1] = y;
  }
  Vector(float x, float y, float z) : c(new float[3]), d(3) {
    c[0] = x;
    c[1] = y;
    c[2] = z;
  }
  Vector(float x, float y, float z, float w) : c(new float[4]), d(4) {
    c[0] = x;
    c[1] = y;
    c[2] = z;
    c[3] = w;
  }
  Vector(const IVector &v);
  //Vector(IVector &&v);
  Vector(const Vector &v);
  Vector(Vector &&v);
  Vector(const Vector &&v);
  Vector &operator=(const IVector &v);
  //Vector &operator=(IVector &&v);
  Vector &operator=(const Vector &v);
  Vector &operator=(Vector &&v);
  Vector &operator=(const Vector &&v);
  ~Vector() {
    if (c)
      delete[] c;
  }
  inline bool valid() const { return c != 0; }
  inline float operator[](int i) const { return c[i]; }
  // inline float operator[](size_t i) const { return c[i]; }
  inline float &operator[](int i) { return c[i]; }
  // inline float &operator[](size_t i) { return c[i]; }
  template <typename F> void forEach(F f) const {
    float *A = new float[d];
    std::copy(c, c + d, A);
    for (int i = 0; i < d; i++) {
      f(c[i], i, A);
    }
  };
  template <typename F> Vector map(F f) const {
    float *A = new float[d];
    std::copy(c, c + d, A);
    float *a = new float[d];
    for (int i = 0; i < d; i++) {
      a[i] = f(c[i], i, A);
    }
    return Vector(d, a);
  };
  template <typename F> Vector &transform(F f) {
    float *A = new float[d];
    std::copy(c, c + d, A);
    for (int i = 0; i < d; i++) {
      c[i] = f(c[i], i, A);
    }
    return *this;
  };
  template <typename F, typename S> S reduce(F f, S s) const {
    float *A = new float[d];
    std::copy(c, c + d, A);
    S a = s;
    for (int i = 0; i < d; i++) {
      a = f(a, c[i], i, A);
    }
    return a;
  };
  template <typename F> bool VectorEvery(F f) const {
    float *A = new float[d];
    std::copy(c, c + d, A);
    for (int i = 0; i < d; i++) {
      if (!f(c[i], i, A))
        return false;
    }
    return true;
  };
  template <typename F> bool VectorSome(F f) const {
    float *A = new float[d];
    std::copy(c, c + d, A);
    for (int i = 0; i < d; i++) {
      if (f(c[i], i, A))
        return true;
    }
    return false;
  };

  Vector &negate();
  Vector negative() const;

  VECTOR_UNARY_FN_SIG(Vector, cos, Vector)
  VECTOR_UNARY_FN_SIG(Vector, sin, Vector)
  VECTOR_UNARY_FN_SIG(Vector, tan, Vector)
  VECTOR_UNARY_FN_SIG(Vector, acos, Vector)
  VECTOR_UNARY_FN_SIG(Vector, asin, Vector)
  VECTOR_UNARY_FN_SIG(Vector, atan, Vector)
  VECTOR_UNARY_FN_SIG(Vector, cosh, Vector)
  VECTOR_UNARY_FN_SIG(Vector, sinh, Vector)
  VECTOR_UNARY_FN_SIG(Vector, tanh, Vector)
  VECTOR_UNARY_FN_SIG(Vector, acosh, Vector)
  VECTOR_UNARY_FN_SIG(Vector, asinh, Vector)
  VECTOR_UNARY_FN_SIG(Vector, atanh, Vector)
  VECTOR_UNARY_FN_SIG(Vector, floor, IVector)
  VECTOR_UNARY_FN_SIG(Vector, ceil, IVector)

#define VECTOR_BINARY_OP_S_SCALAR_SIG(f) Vector &f(const float v);
#define VECTOR_BINARY_OP_S_ARR_SIG(f) Vector &f(const float *v);
#define VECTOR_BINARY_OP_S_VEC_SIG(f) StatusOr<Vector> f(const Vector &v);
#define VECTOR_BINARY_OP_SO_SCALAR_SIG(f) Vector &operator f(const float v);
#define VECTOR_BINARY_OP_SO_ARR_SIG(f) Vector &operator f(const float *v);
#define VECTOR_BINARY_OP_SO_VEC_SIG(f) Vector &operator f(const Vector &v);
#define VECTOR_BINARY_OP_O_SCALAR_SIG(f) Vector f(const float v) const;
#define VECTOR_BINARY_OP_O_ARR_SIG(f) Vector f(const float *v) const;
#define VECTOR_BINARY_OP_O_VEC_SIG(f) StatusOr<Vector> f(const Vector &v) const;
#define VECTOR_BINARY_OP_OO_SCALAR_SIG(op)                                     \
  Vector operator op(const float v) const;
#define VECTOR_BINARY_OP_OO_ARR_SIG(op)                                        \
  Vector operator op(const float *v) const;
#define VECTOR_BINARY_OP_OO_VEC_SIG(op)                                        \
  Vector operator op(const Vector &v) const;

#define VECTOR_BINARY_OP_S_SIG(f)                                              \
  VECTOR_BINARY_OP_S_SCALAR_SIG(f)                                             \
  VECTOR_BINARY_OP_S_ARR_SIG(f)                                                \
  VECTOR_BINARY_OP_S_VEC_SIG(f)

#define VECTOR_BINARY_OP_SO_SIG(f)                                             \
  VECTOR_BINARY_OP_SO_SCALAR_SIG(f)                                            \
  VECTOR_BINARY_OP_SO_ARR_SIG(f)                                               \
  VECTOR_BINARY_OP_SO_VEC_SIG(f)

#define VECTOR_BINARY_OP_O_SIG(f)                                              \
  VECTOR_BINARY_OP_O_SCALAR_SIG(f)                                             \
  VECTOR_BINARY_OP_O_ARR_SIG(f)                                                \
  VECTOR_BINARY_OP_O_VEC_SIG(f)

#define VECTOR_BINARY_OP_OO_SIG(op)                                            \
  VECTOR_BINARY_OP_OO_SCALAR_SIG(op)                                           \
  VECTOR_BINARY_OP_OO_ARR_SIG(op)                                              \
  VECTOR_BINARY_OP_OO_VEC_SIG(op)

#define VECTOR_BINARY_OP_SIG(fs, fso, fo, op)                                  \
  VECTOR_BINARY_OP_S_SIG(fs)                                                   \
  VECTOR_BINARY_OP_SO_SIG(fso)                                                 \
  VECTOR_BINARY_OP_O_SIG(fo)                                                   \
  VECTOR_BINARY_OP_OO_SIG(op)

  VECTOR_BINARY_OP_SIG(add, +=, sum, +)
  VECTOR_BINARY_OP_SIG(subtract, -=, difference, -)
  VECTOR_BINARY_OP_SIG(multiply, *=, product, *)
  VECTOR_BINARY_OP_SIG(divide, /=, quotient, /)

#define VECTOR_BINARY_FN_S_SCALAR_SIG(f) Vector &f(const float v);
#define VECTOR_BINARY_FN_S_ARR_SIG(f) Vector &f(const float *v);
#define VECTOR_BINARY_FN_S_VEC_SIG(f) StatusOr<Vector> f(const Vector &v);
#define VECTOR_BINARY_FN_SO_SCALAR_SIG(f) Vector &operator f(const float v);
#define VECTOR_BINARY_FN_SO_ARR_SIG(f) Vector &operator f(const float *v);
#define VECTOR_BINARY_FN_SO_VEC_SIG(f) Vector &operator f(const Vector &v);
#define VECTOR_BINARY_FN_O_SCALAR_SIG(f) Vector f(const float v) const;
#define VECTOR_BINARY_FN_O_ARR_SIG(f) Vector f(const float *v) const;
#define VECTOR_BINARY_FN_O_VEC_SIG(f) StatusOr<Vector> f(const Vector &v) const;
#define VECTOR_BINARY_FN_OO_SCALAR_SIG(op)                                     \
  Vector operator op(const float v) const;
#define VECTOR_BINARY_FN_OO_ARR_SIG(op)                                        \
  Vector operator op(const float *v) const;
#define VECTOR_BINARY_FN_OO_VEC_SIG(op)                                        \
  Vector operator op(const Vector &v) const;

#define VECTOR_BINARY_FN_S_SIG(f)                                              \
  VECTOR_BINARY_FN_S_SCALAR_SIG(f)                                             \
  VECTOR_BINARY_FN_S_ARR_SIG(f)                                                \
  VECTOR_BINARY_FN_S_VEC_SIG(f)

#define VECTOR_BINARY_FN_SO_SIG(f)                                             \
  VECTOR_BINARY_FN_SO_SCALAR_SIG(f)                                            \
  VECTOR_BINARY_FN_SO_ARR_SIG(f)                                               \
  VECTOR_BINARY_FN_SO_VEC_SIG(f)

#define VECTOR_BINARY_FN_O_SIG(f)                                              \
  VECTOR_BINARY_FN_O_SCALAR_SIG(f)                                             \
  VECTOR_BINARY_FN_O_ARR_SIG(f)                                                \
  VECTOR_BINARY_FN_O_VEC_SIG(f)

#define VECTOR_BINARY_FN_OO_SIG(op)                                            \
  VECTOR_BINARY_FN_OO_SCALAR_SIG(op)                                           \
  VECTOR_BINARY_FN_OO_ARR_SIG(op)                                              \
  VECTOR_BINARY_FN_OO_VEC_SIG(op)

#define VECTOR_BINARY_FN_SIG(fs, fso, fo, op)                                  \
  VECTOR_BINARY_FN_S_SIG(fs)                                                   \
  VECTOR_BINARY_FN_SO_SIG(fso)                                                 \
  VECTOR_BINARY_FN_O_SIG(fo)                                                   \
  VECTOR_BINARY_FN_OO_SIG(op)

  VECTOR_BINARY_FN_SIG(mod, %=, modulus, %)

  bool operator==(const Vector &v) const;
  bool equals(const Vector &v, float tolerance = .0001) const;
  StatusOr<float> dot(const Vector &v) const;
  StatusOr<Vector> crossEquals(const Vector *vs);
  StatusOr<Vector> cross(const Vector *vs) const;
  float length2() const;
  float length() const;
  Vector &normalize();
  Vector unit() const;
  StatusOr<Vector> min(const Vector &v) const;
  StatusOr<Vector> max(const Vector &v) const;
  float min() const;
  float max() const;
  float *toAngles();
  float angleTo(const Vector &a) const;
  float *toArray() const;
  Vector clone() const;
  void set(float *v);
  std::string toStr() const;

  static Vector fromAngles(float *as);
  static Vector randomDirection();
  static Vector randomDirection(const Vector &v, const Vector &w, float factor);

  friend IVector;
};

void floor(Vector *v);
Vector floor(const Vector &v);
Vector &floor(const Vector &v, Vector *w);

} // namespace vector
} // namespace math

#endif // MATH_VECTOR_VECTOR_H