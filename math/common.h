#ifndef MATH_COMMON_H
#define MATH_COMMON_H

#include <cmath>

namespace math {

template <typename T> bool floatEquals(T a, T b, T tolerance = .0001) {
  if (std::isnan(a) && std::isnan(b)) {
    return true;
  }
  if (std::isinf(a) && std::isinf(b)) {
    return true;
  }
  return fabs(a - b) < tolerance;
}

template <typename O = int, typename I = int> O fact(I n) {
  O f = 1;
  for (I i = 2; i < n; i++) {
    f *= i;
  }
  return f;
}

template <typename O = int, typename I = int> O fact2(I n) {
  O f = 1;
  for (I i = 3; i < n; i += 2) {
    f *= i;
  }
  return f;
}

} // namespace math

#endif // MATH_COMMON_H