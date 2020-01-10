#include "math/vector/vector.h"

#include <cmath>
#include <sstream>

#include "math/common.h"

Vector& Vector::negate() {
  return transform([](float e, int i, float *A) { return -e; });
}
Vector Vector::negative() const {
  return map([](float e, int i, float *A) { return -e; });
}
Vector& Vector::add(const float v) {
  return transform([v](float e, int i, float *A) { return e + v; });
}
Vector& Vector::add(const float *v) {
  return transform([v](float e, int i, float *A) { return e + v[i]; });
}
Vector& Vector::add(const Vector& v) {
  // if (v.d != d) return;
  return transform([v](float e, int i, float *A) { return e + v[i]; });
}
Vector Vector::sum(const float v) const {
  return map([v](float e, int i, float *A) { return e + v; });
}
Vector Vector::sum(const float *v) const {
  return map([v](float e, int i, float *A) { return e + v[i]; });
}
Vector Vector::sum(const Vector& v) const {
  // if (v.d != d) return;
  return map([v](float e, int i, float *A) { return e + v[i]; });
}
Vector& Vector::subtract(const float v) {
  return transform([v](float e, int i, float *A) { return e - v; });
}
Vector& Vector::subtract(const float *v) {
  return transform([v](float e, int i, float *A) { return e - v[i]; });
}
Vector& Vector::subtract(const Vector& v) {
  // if (v.d != d) return;
  return transform([v](float e, int i, float *A) { return e - v[i]; });
}
Vector Vector::difference(const float v) const {
  return map([v](float e, int i, float *A) { return e - v; });
}
Vector Vector::difference(const float *v) const {
  return map([v](float e, int i, float *A) { return e - v[i]; });
}
Vector Vector::difference(const Vector& v) const {
  // if (v.d != d) return;
  return map([v](float e, int i, float *A) { return e - v[i]; });
}
Vector& Vector::multiply(const float v) {
  return transform([v](float e, int i, float *A) { return e * v; });
}
Vector& Vector::multiply(const float *v) {
  return transform([v](float e, int i, float *A) { return e * v[i]; });
}
Vector& Vector::multiply(const Vector& v) {
  // if (v.d != d) return;
  return transform([v](float e, int i, float *A) { return e * v[i]; });
}
Vector Vector::product(const float v) const {
  return map([v](float e, int i, float *A) { return e * v; });
}
Vector Vector::product(const float *v) const {
  return map([v](float e, int i, float *A) { return e * v[i]; });
}
Vector Vector::product(const Vector& v) const {
  // if (v.d != d) return;
  return map([v](float e, int i, float *A) { return e * v[i]; });
}
Vector& Vector::divide(const float v) {
  return transform([v](float e, int i, float *A) { return e / v; });
}
Vector& Vector::divide(const float *v) {
  return transform([v](float e, int i, float *A) { return e / v[i]; });
}
Vector& Vector::divide(const Vector& v) {
  // if (v.d != d) return;
  return transform([v](float e, int i, float *A) { return e / v[i]; });
}
Vector Vector::quotient(const float v) const {
  return map([v](float e, int i, float *A) { return e / v; });
}
Vector Vector::quotient(const float *v) const {
  return map([v](float e, int i, float *A) { return e / v[i]; });
}
Vector Vector::quotient(const Vector& v) const {
  // if (v.d != d) return;
  return map([v](float e, int i, float *A) { return e / v[i]; });
}
bool Vector::equals(const Vector& v, float tolerance) const {
  return reduce([v, tolerance](bool a, float e, int i, float *A) { return a && floatEquals(e, v[i], tolerance); }, true);
}
float Vector::dot(const Vector& v) const {
  return reduce([v](float a, float e, int i, float *A) { return a + e * v[i]; }, 0);
}
/*
Vector& Vector::crossEquals(const Vector *vs) {
  if (d == 2) {
    var x = x;
    x = y;
    y = -x;
    return this;
  }
  var rows = _.times(d, i => i ? (i == 1 ? this : (_.isArray(vs) ? vs[i - 2] : vs)).toArray() : _.times(d, () => 0));
  for (var i = 0; i < d; i++) {
    this[i] = cofactor(rows, 0, i);
  }
  return this;
}
Vector Vector::cross(const Vector *vs) {
  if (d == 2) {
    return new Vector(2, y, -x);
  }
  var rows = _.times(d, i => i ? (i == 1 ? this : (_.isArray(vs) ? vs[i - 2] : vs)).toArray() : _.times(d, () => 0));
  return map((e, i) => cofactor(rows, 0, i));
}
//*/
float Vector::length() const {
  return sqrt(dot(*this));
}
Vector& Vector::normalize() {
  return divide(length());
}
Vector Vector::unit() const {
  return quotient(length());
}
float Vector::min() const {
  return reduce([](float a, float e, int i, float *A) { return fmin(a, e); }, v[0]);
}
float Vector::max() const {
  return reduce([](float a, float e, int i, float *A) { return fmax(a, e); }, v[0]);
}
/*
float *Vector::toAngles() {
  return {
    theta: atan2(z, x),
    phi: asin(y / length())
  };
}
//*/
float Vector::angleTo(const Vector& a) const {
  return acos(dot(a) / (length() * a.length()));
}
float *Vector::toArray() const {
  float *A = new float[d];
  std::copy(v, v + d, A);
  return A;
}
Vector Vector::clone() const {
  return Vector(d, toArray());
}
void Vector::set(float *v) {
  std::copy(v, v + d, this->v);
}
std::string Vector::toStr() const {
  std::stringstream ss;
  ss << "Vector" << d << '[';
  ss << v[0];
  for(int i = 1; i < d; i++) {
    ss << ',' << v[i];
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