#ifndef MATH_GEOMETRY_SHAPES_POLYGON_H
#define MATH_GEOMETRY_SHAPES_POLYGON_H

#include "math/geometry/shapes/shape.h"
#include "math/vector/vector.h"

#include <vector>

using math::vector::Vector;

namespace math {
namespace geometry {
namespace shapes {

class FPolygon : public Shape {
public:
  bool Inside(vector::IVector point) override;
  bool Inside(vector::FVector point) override;
  bool Inside(vector::DVector point) override;
  float Intersect(vector::ICube cube) override;
  float Intersect(vector::FCube cube) override;
  double Intersect(vector::DCube cube) override;

private:
  std::vector<Vector> points_;
};

} // namespace shapes
} // namespace geometry
} // namespace math

#endif // MATH_GEOMETRY_SHAPES_POLYGON_H