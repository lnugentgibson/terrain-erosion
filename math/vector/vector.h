#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <string>

class Vector {
  float *v;
 public:
  const int d;
  Vector(int _d) : v(new float[_d]), d(_d) {}
  Vector(int _d, float *_v) : v(_v), d(_d) {}
  float operator [](int i) const {
    return v[i];
  }
  float& operator [](int i) {
    return v[i];
  }
  template<typename F>
  void forEach(F f) {
    float *A = new float[d];
    std::copy(v, v + d, A);
    for (int i = 0; i < d; i++) {
      f(v[i], i, A);
    }
  };
  template<typename F>
  Vector map(F f) {
    float *A = new float[d];
    std::copy(v, v + d, A);
    float *a = new float[d];
    for (int i = 0; i < d; i++) {
      a[i] = f(v[i], i, A);
    }
    return Vector(d, a);
  };
  template<typename F>
  Vector& transform(F f) {
    float *A = new float[d];
    std::copy(v, v + d, A);
    for (int i = 0; i < d; i++) {
      v[i] = f(v[i], i, A);
    }
    return *this;
  };
  template<typename F, typename S>
  S reduce(F f, S s) {
    float *A = new float[d];
    std::copy(v, v + d, A);
    S a = s;
    for (int i = 0; i < d; i++) {
      a = f(a, v[i], i, A);
    }
    return a;
  };
  Vector& negate();
  Vector negative();
  Vector& add(const float v);
  Vector& add(const float *v);
  Vector& add(const Vector& v);
  Vector sum(const float v);
  Vector sum(const float *v);
  Vector sum(const Vector& v);
  Vector& subtract(const float v);
  Vector& subtract(const float *v);
  Vector& subtract(const Vector& v);
  Vector difference(const float v);
  Vector difference(const float *v);
  Vector difference(const Vector& v);
  Vector& multiply(const float v);
  Vector& multiply(const float *v);
  Vector& multiply(const Vector& v);
  Vector product(const float v);
  Vector product(const float *v);
  Vector product(const Vector& v);
  Vector& divide(const float v);
  Vector& divide(const float *v);
  Vector& divide(const Vector& v);
  Vector quotient(const float v);
  Vector quotient(const float *v);
  Vector quotient(const Vector& v);
  bool equals(const Vector& v, float tolerance = .0001);
  float dot(const Vector& v);
  Vector& crossEquals(const Vector *vs);
  Vector cross(const Vector *vs);
  float length();
  Vector normalize();
  Vector& unit();
  float min();
  float max();
  float *toAngles();
  float angleTo(const Vector& a);
  float *toArray();
  Vector clone();
  void set(void *v);
  std::string toStr();
};

#endif // VECTOR_H