#include "experimental/users/nugentgibson/container/spatial/spatial_container.h"

using math::geometry::shapes::Polygon;
using math::geometry::shapes::Rectangle;
using math::vector::Vector;
using util::Status;
using util::StatusOr;

namespace container {
namespace spatial {

bool operator<(const Vector &v, const Vector &w) {
  for (int i = 0; i < v.dimension(); i++) {
    if (v[i] < w[i]) {
      return true;
    } else if (v[i] > w[i]) {
      return false;
    }
  }
  return false;
}

size_t SimpleSpatialContainer::Size() const { return points_.size(); }
std::optional<Rectangle> SimpleSpatialContainer::Bounds() const {
  return std::nullopt;
}
StatusOr<Rectangle> SimpleSpatialContainer::Range() const {
  return absl::Status(absl::StatusCode::kUnimplemented, "");
}
StatusOr<Polygon> SimpleSpatialContainer::Hull() const {
  return absl::Status(absl::StatusCode::kUnimplemented, "");
}

size_t SimpleSpatialContainer::Insert(Vector p) { return 0; }
// std::vector<size_t> SimpleSpatialContainer::add_all(std::vector<Vector> ps);
Status SimpleSpatialContainer::Remove(size_t id) { return absl::OkStatus(); }
Status SimpleSpatialContainer::RemoveAll(std::vector<size_t> ids) {
  return absl::OkStatus();
}
Status SimpleSpatialContainer::Clear() { return absl::OkStatus(); }

// Vector SimpleSpatialContainer::get(size_t id) const;

/*
StatusOr<size_t> SimpleSpatialContainer::NearestNeighbor(Vector q) const;
StatusOr<std::vector<size_t>>
SimpleSpatialContainer::NNearestNeighbor(Vector q, int n) const;
Status
SimpleSpatialContainer::NNearestNeighbor(Vector q, int n,
                                           std::vector<size_t> ids) const;
StatusOr<std::vector<size_t>>
SimpleSpatialContainer::SearchPrism(Vector b, Vector s) const;
Status SimpleSpatialContainer::SearchPrism(Vector b, Vector s,
                                            std::vector<size_t> ids) const;
StatusOr<std::vector<size_t>>
SimpleSpatialContainer::SearchBall(Vector b, float r) const;
Status SimpleSpatialContainer::SearchBall(Vector b, float r,
                                           std::vector<size_t> ids) const;
*/

} // namespace spatial
} // namespace container
