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
  void forEach(F f) const {
    float *A = new float[d];
    std::copy(v, v + d, A);
    for (int i = 0; i < d; i++) {
      f(v[i], i, A);
    }
  };
  template<typename F>
  Vector map(F f) const {
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
  S reduce(F f, S s) const {
    float *A = new float[d];
    std::copy(v, v + d, A);
    S a = s;
    for (int i = 0; i < d; i++) {
      a = f(a, v[i], i, A);
    }
    return a;
  };
  Vector& negate();
  Vector negative() const;
  Vector& add(const float v);
  Vector& add(const float *v);
  Vector& add(const Vector& v);
  Vector sum(const float v) const;
  Vector sum(const float *v) const;
  Vector sum(const Vector& v) const;
  Vector& subtract(const float v);
  Vector& subtract(const float *v);
  Vector& subtract(const Vector& v);
  Vector difference(const float v) const;
  Vector difference(const float *v) const;
  Vector difference(const Vector& v) const;
  Vector& multiply(const float v);
  Vector& multiply(const float *v);
  Vector& multiply(const Vector& v);
  Vector product(const float v) const;
  Vector product(const float *v) const;
  Vector product(const Vector& v) const;
  Vector& divide(const float v);
  Vector& divide(const float *v);
  Vector& divide(const Vector& v);
  Vector quotient(const float v) const;
  Vector quotient(const float *v) const;
  Vector quotient(const Vector& v) const;
  bool equals(const Vector& v, float tolerance = .0001) const;
  float dot(const Vector& v) const;
  Vector& crossEquals(const Vector *vs);
  Vector cross(const Vector *vs) const;
  float length() const;
  Vector& normalize();
  Vector unit() const;
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

#endif // VECTOR_H