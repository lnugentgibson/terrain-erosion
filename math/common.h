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

} // namespace math

#endif // MATH_COMMON_H