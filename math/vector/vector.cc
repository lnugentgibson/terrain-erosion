#include "math/vector/vector.h"

#include <cmath>

bool floatEquals(float a, float b, float t = .0001) {
  return fabs(a - b) < t;
}

Vector& Vector::negate() {
  return transform([](float e, int i, float *A) { return -e; });
}
Vector Vector::negative() {
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
Vector Vector::sum(const float v) {
  return map([v](float e, int i, float *A) { return e + v; });
}
Vector Vector::sum(const float *v) {
  return map([v](float e, int i, float *A) { return e + v[i]; });
}
Vector Vector::sum(const Vector& v) {
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
Vector Vector::difference(const float v) {
  return map([v](float e, int i, float *A) { return e - v; });
}
Vector Vector::difference(const float *v) {
  return map([v](float e, int i, float *A) { return e - v[i]; });
}
Vector Vector::difference(const Vector& v) {
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
Vector Vector::product(const float v) {
  return map([v](float e, int i, float *A) { return e * v; });
}
Vector Vector::product(const float *v) {
  return map([v](float e, int i, float *A) { return e * v[i]; });
}
Vector Vector::product(const Vector& v) {
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
Vector Vector::quotient(const float v) {
  return map([v](float e, int i, float *A) { return e / v; });
}
Vector Vector::quotient(const float *v) {
  return map([v](float e, int i, float *A) { return e / v[i]; });
}
Vector Vector::quotient(const Vector& v) {
  // if (v.d != d) return;
  return map([v](float e, int i, float *A) { return e / v[i]; });
}
bool Vector::equals(const Vector& v, float tolerance) {
  return reduce([v, tolerance](bool a, float e, int i, float *A) { return a && floatEquals(e, v[i], tolerance); }, true);
}
/*
float Vector::dot(const Vector& v);
Vector& Vector::crossEquals(const Vector *vs);
Vector Vector::cross(const Vector *vs);
float Vector::length();
Vector Vector::normalize();
Vector& Vector::unit();
float Vector::min();
float Vector::max();
float *Vector::toAngles();
float Vector::angleTo(const Vector& a);
float *Vector::toArray();
Vector Vector::clone();
void Vector::set(void *v);
std::string Vector::toStr();
//*/

/*
dot: function(v) {
  return reduce((a, e, i) => a + e * v[i], 0);
},
crossEquals: function(vs) {
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
},
cross: function(vs) {
  if (d == 2) {
    return new Vector(2, y, -x);
  }
  var rows = _.times(d, i => i ? (i == 1 ? this : (_.isArray(vs) ? vs[i - 2] : vs)).toArray() : _.times(d, () => 0));
  return map((e, i) => cofactor(rows, 0, i));
},
length: function() {
  return sqrt(dot(this));
},
normalize: function() {
  return quotient(length());
},
unit: function() {
  return divide(length());
},
min: function() {
  return reduce((a, e) => min(a, e), x);
},
max: function() {
  return reduce((a, e) => max(a, e), x);
},
toAngles: function() {
  return {
    theta: atan2(z, x),
    phi: asin(y / length())
  };
},
angleTo: function(a) {
  return acos(dot(a) / (length() * a.length()));
},
toArray: function(n) {
  return _.times(d, i => this[i]);
},
clone: function() {
  return new Vector(d, toArray());
},
new: function(d, x, y, z, w, v) {
  return new Vector(d, x, y, z, w, v);
},
set: function(x, y, z, w, v) {
  x = x;
  y = y;
  z = z;
  w = w;
  if (v)
    for (var i = 0; i < v.length; i++)
      v[i + 4] = v[i];
  return this;
},
toStr: function() {
  return `Vector${d}[${toArray().join(',')}]`;
}
//*/