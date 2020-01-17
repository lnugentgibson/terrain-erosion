#ifndef MATH_VECTOR_VECTOR_H
#define MATH_VECTOR_VECTOR_H

#include <algorithm>
#include <string>

#include "util/statusor.h"

using util::StatusOr;

namespace math {
namespace vector {
  
class Matrix;

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
  float operator [](int i) const {
    return c[i];
  }
  float& operator [](int i) {
    return c[i];
  }
  template<typename F>
  void forEach(F f) const {
    float *A = new float[d];
    std::copy(c, c + d, A);
    for (int i = 0; i < d; i++) {
      f(c[i], i, A);
    }
  };
  template<typename F>
  Vector map(F f) const {
    float *A = new float[d];
    std::copy(c, c + d, A);
    float *a = new float[d];
    for (int i = 0; i < d; i++) {
      a[i] = f(c[i], i, A);
    }
    return Vector(d, a);
  };
  template<typename F>
  Vector& transform(F f) {
    float *A = new float[d];
    std::copy(c, c + d, A);
    for (int i = 0; i < d; i++) {
      c[i] = f(c[i], i, A);
    }
    return *this;
  };
  template<typename F, typename S>
  S reduce(F f, S s) const {
    float *A = new float[d];
    std::copy(c, c + d, A);
    S a = s;
    for (int i = 0; i < d; i++) {
      a = f(a, c[i], i, A);
    }
    return a;
  };
  Vector& negate();
  Vector negative() const;
  Vector& add(const float v);
  Vector& add(const float *v);
  StatusOr<Vector> add(const Vector& v);
  Vector& operator +=(const float v);
  Vector& operator +=(const float *v);
  Vector& operator +=(const Vector& v);
  Vector sum(const float v) const;
  Vector sum(const float *v) const;
  StatusOr<Vector> sum(const Vector& v) const;
  Vector operator +(const float v);
  Vector operator +(const float *v);
  Vector operator +(const Vector& v);
  Vector& subtract(const float v);
  Vector& subtract(const float *v);
  StatusOr<Vector> subtract(const Vector& v);
  Vector& operator -=(const float v);
  Vector& operator -=(const float *v);
  Vector& operator -=(const Vector& v);
  Vector difference(const float v) const;
  Vector difference(const float *v) const;
  StatusOr<Vector> difference(const Vector& v) const;
  Vector operator -(const float v);
  Vector operator -(const float *v);
  Vector operator -(const Vector& v);
  Vector& multiply(const float v);
  Vector& multiply(const float *v);
  StatusOr<Vector> multiply(const Vector& v);
  Vector& operator *=(const float v);
  Vector& operator *=(const float *v);
  Vector& operator *=(const Vector& v);
  Vector product(const float v) const;
  Vector product(const float *v) const;
  StatusOr<Vector> product(const Vector& v) const;
  Vector operator *(const float v);
  Vector operator *(const float *v);
  Vector operator *(const Vector& v);
  Vector& divide(const float v);
  Vector& divide(const float *v);
  StatusOr<Vector> divide(const Vector& v);
  Vector& operator /=(const float v);
  Vector& operator /=(const float *v);
  Vector& operator /=(const Vector& v);
  Vector quotient(const float v) const;
  Vector quotient(const float *v) const;
  StatusOr<Vector> quotient(const Vector& v) const;
  Vector operator /(const float v);
  Vector operator /(const float *v);
  Vector operator /(const Vector& v);
  bool equals(const Vector& v, float tolerance = .0001) const;
  StatusOr<float> dot(const Vector& v) const;
  StatusOr<Vector> crossEquals(const Vector *vs);
  StatusOr<Vector> cross(const Vector *vs) const;
  float length2() const;
  float length() const;
  Vector& normalize();
  Vector unit() const;
  StatusOr<Vector> min(const Vector& v) const;
  StatusOr<Vector> max(const Vector& v) const;
  float min() const;
  float max() const;
  float *toAngles();
  float angleTo(const Vector& a) const;
  float *toArray() const;
  Vector clone() const;
  void set(float *v);
  std::string toStr() const;
  
  static Vector fromAngles(float *as);
  static Vector randomDirection();
  static Vector randomDirection(const Vector& v, const Vector& w, float factor);
};

} // namespace vector
} // namespace math

#endif // MATH_VECTOR_VECTOR_H