#include "math/geometry/shapes/shape.h"

#include <algorithm>

namespace math {
namespace geometry {
namespace shapes {

vector::Vector Rectangle::min() {
  auto c = corner + size;
  vector::Vector n(2);
  n[0] = std::min(corner[0], c[0]);
  n[1] = std::min(corner[1], c[1]);
  return n;
}

vector::Vector Rectangle::max() {
  auto c = corner + size;
  vector::Vector x(2);
  x[0] = std::max(corner[0], c[0]);
  x[1] = std::max(corner[1], c[1]);
  return x;
}

bool Rectangle::Inside(vector::Vector p) {
  auto n = min();
  auto x = max();
  if(p[0] < n[0] || p[1] < n[1]) return false;
  if(p[0] > x[0] || p[1] > x[1]) return false;
  return true;
}

} // namespace shapes
} // namespace geometry
} // namespace math