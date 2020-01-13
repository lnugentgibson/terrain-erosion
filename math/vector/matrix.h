#ifndef MATH_VECTOR_MATRIX_H
#define MATH_VECTOR_MATRIX_H

//#include <algorithm>
#include <string>

class Matrix {
  float *c;
 public:
  const int m, n;
  Matrix(int _m, int _n) : c(new float[_m * _n]), m(_m), n(_n) {}
  Matrix(int _m, int _n, float *_c) : c(_c), m(_m), n(_n) {}
  /*
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
  Matrix map(F f) const {
    float *A = new float[d];
    std::copy(v, v + d, A);
    float *a = new float[d];
    for (int i = 0; i < d; i++) {
      a[i] = f(v[i], i, A);
    }
    return Matrix(d, a);
  };
  template<typename F>
  Matrix& transform(F f) {
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
  //*/
  Matrix& negate();
  Matrix negative() const;
  Matrix& add(const float v);
  Matrix& add(const float *v);
  Matrix& add(const Matrix& v);
  Matrix sum(const float v) const;
  Matrix sum(const float *v) const;
  Matrix sum(const Matrix& v) const;
  Matrix& subtract(const float v);
  Matrix& subtract(const float *v);
  Matrix& subtract(const Matrix& v);
  Matrix difference(const float v) const;
  Matrix difference(const float *v) const;
  Matrix difference(const Matrix& v) const;
  Matrix& multiply(const float v);
  Matrix& multiply(const float *v);
  Matrix& multiply(const Matrix& v);
  Matrix product(const float v) const;
  Matrix product(const float *v) const;
  Matrix product(const Matrix& v) const;
  Matrix& divide(const float v);
  Matrix& divide(const float *v);
  Matrix& divide(const Matrix& v);
  Matrix quotient(const float v) const;
  Matrix quotient(const float *v) const;
  Matrix quotient(const Matrix& v) const;
  bool equals(const Matrix& v, float tolerance = .0001) const;
  float min() const;
  float max() const;
  float *toArray() const;
  Matrix clone() const;
  void set(float *v);
  std::string toStr() const;
};

#endif // MATH_VECTOR_MATRIX_H