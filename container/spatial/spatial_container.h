#ifndef EXPERIMENTAL_USERS_NUGENTGIBSON_CONTAINER_SPATIAL_SPATIAL_CONTAINER_H_
#define EXPERIMENTAL_USERS_NUGENTGIBSON_CONTAINER_SPATIAL_SPATIAL_CONTAINER_H_

#include <map>
#include <vector>

#include "experimental/users/nugentgibson/math/geometry/shapes/shape.h"
#include "experimental/users/nugentgibson/math/vector/vector.h"
#include "third_party/absl/status/status.h"
#include "util/task/statusor.h"

namespace container {
namespace spatial {

class SpatialContainer {
public:
  explicit SpatialContainer(int d) : d_(d) {}
  virtual ~SpatialContainer() {}

  inline int Dimension() const { return d_; }

  virtual size_t Size() const = 0;
  // The allowed region for points to exist. If set, points cannot be added
  // outside this range.
  virtual std::optional<math::geometry::shapes::Rectangle> Bounds() const = 0;
  virtual util::StatusOr<math::geometry::shapes::Rectangle> Range() const = 0;
  virtual util::StatusOr<math::geometry::shapes::Polygon> Hull() const = 0;

  virtual size_t Insert(math::vector::Vector p) = 0;
  virtual std::vector<size_t>
  InsertAll(std::vector<math::vector::Vector> ps) = 0;
  virtual absl::Status Remove(size_t id) = 0;
  virtual absl::Status RemoveAll(std::vector<size_t> ids) = 0;
  virtual absl::Status Clear() = 0;

  virtual math::vector::Vector Get(size_t id) const = 0;

  virtual util::StatusOr<size_t>
  NearestNeighbor(math::vector::Vector q) const = 0;
  virtual util::StatusOr<std::vector<size_t>>
  NNearestNeighbor(math::vector::Vector q, int n) const = 0;
  virtual absl::Status NNearestNeighbor(math::vector::Vector q, int n,
                                        std::vector<size_t> ids) const = 0;
  virtual util::StatusOr<std::vector<size_t>>
  SearchPrism(math::vector::Vector b, math::vector::Vector s) const = 0;
  virtual absl::Status SearchPrism(math::vector::Vector b,
                                   math::vector::Vector s,
                                   std::vector<size_t> ids) const = 0;
  virtual util::StatusOr<std::vector<size_t>> SearchBall(math::vector::Vector b,
                                                         float r) const = 0;
  virtual absl::Status SearchBall(math::vector::Vector b, float r,
                                  std::vector<size_t> ids) const = 0;

private:
  int d_;
};

class SimpleSpatialContainer : public SpatialContainer {
public:
  explicit SimpleSpatialContainer(int d) : SpatialContainer(d) {}

  size_t Size() const override;
  std::optional<math::geometry::shapes::Rectangle> Bounds() const override;
  util::StatusOr<math::geometry::shapes::Rectangle> Range() const override;
  util::StatusOr<math::geometry::shapes::Polygon> Hull() const override;

  size_t Insert(math::vector::Vector p) override;
  std::vector<size_t> InsertAll(std::vector<math::vector::Vector> ps) override;
  absl::Status Remove(size_t id) override;
  absl::Status RemoveAll(std::vector<size_t> ids) override;
  absl::Status Clear() override;

  math::vector::Vector Get(size_t id) const override;

  util::StatusOr<size_t> NearestNeighbor(math::vector::Vector q) const override;
  util::StatusOr<std::vector<size_t>> NNearestNeighbor(math::vector::Vector q,
                                                       int n) const override;
  absl::Status NNearestNeighbor(math::vector::Vector q, int n,
                                std::vector<size_t> ids) const override;
  util::StatusOr<std::vector<size_t>>
  SearchPrism(math::vector::Vector b, math::vector::Vector s) const override;
  absl::Status SearchPrism(math::vector::Vector b, math::vector::Vector s,
                           std::vector<size_t> ids) const override;
  util::StatusOr<std::vector<size_t>> SearchBall(math::vector::Vector b,
                                                 float r) const override;
  absl::Status SearchBall(math::vector::Vector b, float r,
                          std::vector<size_t> ids) const override;

private:
  std::map<math::vector::Vector, size_t> points_;
};

} // namespace spatial
} // namespace container

#endif // EXPERIMENTAL_USERS_NUGENTGIBSON_CONTAINER_SPATIAL_SPATIAL_CONTAINER_H_
