#include "math/geometry/shapes/shape.h"

#include <algorithm>

namespace math {
namespace geometry {
namespace shapes {

bool InsideFRectangle(vector::FVector point, vector::FVector corner,
                      vector::FVector size) {
  if (point[0] < corner[0] || point[1] < corner[1])
    return false;
  if (point[0] > corner[0] + size[0] || point[1] > corner[1] + size[1])
    return false;
  return true;
}

FRectangle::FRectangle(vector::FVector c, vector::FVector s)
    : vector::FCube(c, s) {
  normalize();
}

bool FRectangle::Inside(vector::FVector point) {
  return InsideFRectangle(point, start_, difference());
}

float FRectangle::Intersect(vector::FCube cube) {
  vector::FVector d =
      end_.min(cube.end()).ValueOrDie() - start_.max(cube.start()).ValueOrDie();
  return std::max(0.0f, d[0]) * std::max(0.0f, d[1]);
}

bool InsideCircle(vector::FVector point, vector::FVector center, float radius) {
  auto d = point - center;
  return d.length() <= radius;
}

bool FCircle::Inside(vector::FVector point) {
  return InsideCircle(point, center_, radius_);
}

float FCircle::Intersect(vector::FCube cube) {
  return InsideCircle(cube.start(), center_, radius_) ? 1.0 : 0.0;
}

void FCircleField::Push(vector::FVector center) { centers_.push_back(center); }

bool FCircleField::Inside(vector::FVector point) {
  for (auto &center : centers_) {
    if (InsideCircle(point, center, radius_))
      return true;
  }
  return false;
}

float FCircleField::Intersect(vector::FCube cube) {
  for (auto &center : centers_) {
    if (InsideCircle(cube.start(), center, radius_))
      return 1.0;
  }
  return 0.0;
}

} // namespace shapes
} // namespace geometry
} // namespace math