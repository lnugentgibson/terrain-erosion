#ifndef MATH_GEOMETRY_SHAPES_SHAPE_H
#define MATH_GEOMETRY_SHAPES_SHAPE_H

#include "math/vector/vector.h"

#include <vector>

namespace math {
namespace geometry {
namespace shapes {

class Shape {
 public:
  virtual bool Inside(vector::Vector p) = 0;
};

class Rectangle : public Shape {
  vector::Vector corner;
  vector::Vector size;
 public:
  vector::Vector min();
  vector::Vector max();
  Rectangle(vector::Vector c, vector::Vector s) : corner(c), size(s) {}
  bool Inside(vector::Vector p);
};

class Polygon : public Shape {
  std::vector<vector::Vector> points;
 public:
  bool Inside(vector::Vector p);
};

} // namespace shapes
} // namespace geometry
} // namespace math

#endif // MATH_GEOMETRY_SHAPES_SHAPE_H