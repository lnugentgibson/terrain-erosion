#ifndef MATH_GEOMETRY_SHAPES_SHAPE_H
#define MATH_GEOMETRY_SHAPES_SHAPE_H

#include "math/vector/vector.h"

#include <vector>

using math::vector::Vector;

namespace math {
namespace geometry {
namespace shapes {

class Shape {
public:
  virtual bool Inside(Vector p) = 0;
  virtual float Intersect(Vector c, Vector s) = 0;
};

class Rectangle : public Shape {
  Vector corner;
  Vector size;

public:
  Rectangle(Vector c, Vector s);
  bool Inside(Vector p) override;
  float Intersect(Vector c, Vector s) override;
};

class Circle : public Shape {
  Vector center;
  float radius;

public:
  Circle(Vector c, float r) : center(c), radius(r) {}
  bool Inside(Vector p) override;
  float Intersect(Vector c, Vector s) override;
};

class CircleField : public Shape {
  std::vector<Vector> centers;
  float radius;

public:
  CircleField(float r) : radius(r) {}
  void Push(Vector center);
  bool Inside(Vector p) override;
  float Intersect(Vector c, Vector s) override;
};

class Polygon : public Shape {
  std::vector<Vector> points;

public:
  bool Inside(Vector p);
};

} // namespace shapes
} // namespace geometry
} // namespace math

#endif // MATH_GEOMETRY_SHAPES_SHAPE_H