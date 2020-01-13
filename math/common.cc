#include "math/common.h"

#include <cmath>

namespace math {

bool floatEquals(float a, float b, float tolerance) {
  return fabs(a - b) < tolerance;
}

} // namespace math