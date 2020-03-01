#ifndef MATH_GEOMETRY_SHAPES_POLYGON_H
#define MATH_GEOMETRY_SHAPES_POLYGON_H

#include "math/geometry/shapes/shape.h"
#include "math/vector/vector.h"

#include <vector>

using math::vector::Vector;

namespace math {
namespace geometry {
namespace shapes {

class Polygon : public Shape {
public:
  bool Inside(Vector p) override;
  float Intersect(Vector c, Vector s) override;

private:
  std::vector<Vector> points;
};

} // namespace shapes
} // namespace geometry
} // namespace math

#endif // MATH_GEOMETRY_SHAPES_POLYGON_H