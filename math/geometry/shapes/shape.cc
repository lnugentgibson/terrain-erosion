#include "math/geometry/shapes/shape.h"

#include <algorithm>

namespace math {
namespace geometry {
namespace shapes {

bool InsideRectangle(Vector p, Vector corner, Vector size) {
  if(p[0] < corner[0] || p[1] < corner[1]) return false;
  if(p[0] > corner[0] + size[0] || p[1] > corner[1] + size[1]) return false;
  return true;
}

Rectangle::Rectangle(Vector c, Vector s) : corner(2), size(2) {
  auto c2 = c + s;
  corner[0] = std::min(c[0], c2[0]);
  corner[1] = std::min(c[1], c2[1]);
  size[0] = std::max(c[0], c2[0]) - corner[0];
  size[1] = std::max(c[1], c2[1]) - corner[1];
}

bool Rectangle::Inside(Vector p) {
  return InsideRectangle(p, corner, size);
}

float Rectangle::Intersect(Vector c, Vector s) {
  Vector c2 = c + s;
  Vector corner2 = corner + size;
  Vector d = corner2.min(c2) - corner.max(c);
  return std::max(0.0f, d[0]) * std::max(0.0f, d[1]);
}

bool InsideCircle(Vector p, Vector center, float radius) {
  auto d = p - center;
  return d.length() <= radius;
}

bool Circle::Inside(Vector p) {
  return InsideCircle(p, center, radius);
}

float Circle::Intersect(Vector c, Vector s) {
  return InsideCircle(c, center, radius) ? 1.0 : 0.0;
}

void CircleField::Push(Vector center) {
  centers.push_back(center);
}

bool CircleField::Inside(Vector p) {
  for(auto& center : centers) {
    if(InsideCircle(p, center, radius)) return true;
  }
  return false;
}

float CircleField::Intersect(Vector c, Vector s) {
  for(auto& center : centers) {
    if(InsideCircle(c, center, radius)) return 1.0;
  }
  return 0.0;
}

} // namespace shapes
} // namespace geometry
} // namespace math