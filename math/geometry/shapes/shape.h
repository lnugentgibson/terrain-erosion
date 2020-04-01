#ifndef MATH_GEOMETRY_SHAPES_SHAPE_H
#define MATH_GEOMETRY_SHAPES_SHAPE_H

#include "math/vector/vector.h"

#include <vector>

namespace math {
namespace geometry {
namespace shapes {

class Shape {
public:
  virtual bool Inside(vector::IVector point) = 0;
  virtual bool Inside(vector::FVector point) = 0;
  virtual bool Inside(vector::DVector point) = 0;
  virtual float Intersect(vector::ICube cube) = 0;
  virtual float Intersect(vector::FCube cube) = 0;
  virtual double Intersect(vector::DCube cube) = 0;
};

class FRectangle : public Shape, public vector::FCube {
public:
  FRectangle(vector::FVector corner, vector::FVector size);
  bool Inside(vector::IVector point) override;
  bool Inside(vector::FVector point) override;
  bool Inside(vector::DVector point) override;
  float Intersect(vector::ICube cube) override;
  float Intersect(vector::FCube cube) override;
  double Intersect(vector::DCube cube) override;
};

class FCircle : public Shape, public vector::FCircle {
public:
  FCircle(vector::FVector c, float r) : vector::FCircle(c, r) {}
  bool Inside(vector::IVector point) override;
  bool Inside(vector::FVector point) override;
  bool Inside(vector::DVector point) override;
  float Intersect(vector::ICube cube) override;
  float Intersect(vector::FCube cube) override;
  double Intersect(vector::DCube cube) override;
};

class FCircleField : public Shape {
  std::vector<vector::FVector> centers_;
  float radius_;

public:
  FCircleField(float r) : radius_(r) {}
  void Push(vector::FVector center);
  bool Inside(vector::IVector point) override;
  bool Inside(vector::FVector point) override;
  bool Inside(vector::DVector point) override;
  float Intersect(vector::ICube cube) override;
  float Intersect(vector::FCube cube) override;
  double Intersect(vector::DCube cube) override;
};

} // namespace shapes
} // namespace geometry
} // namespace math

#endif // MATH_GEOMETRY_SHAPES_SHAPE_H