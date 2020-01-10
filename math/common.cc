#include "math/common.h"

#include <cmath>

bool floatEquals(float a, float b, float tolerance) {
  return fabs(a - b) < tolerance;
}