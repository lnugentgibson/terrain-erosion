#ifndef CONTAINER_SPATIAL_SPATIAL_CONTAINER_H
#define CONTAINER_SPATIAL_SPATIAL_CONTAINER_H

#include <map>
#include <vector>

#include "math/vector/vector.h"
#include "util/status.h"
#include "util/statusor.h"

namespace container {
namespace spatial {
  
  using util::Status;
  using util::StatusOr;
  using math::vector::Vector;

class SpatialContainer {
public:
  SpatialContainer(int d) : d_(d) {}

  inline int dim() const { return d_; }

  virtual size_t size() const = 0;
  virtual std::pair<Vector, Vector> bounds() const = 0;
  virtual StatusOr<std::pair<Vector, Vector>> range() const = 0;

  virtual size_t add(Vector p) = 0;
  virtual std::vector<size_t> add_all(std::vector<Vector> ps) = 0;
  virtual Status remove(size_t id) = 0;
  virtual Status remove_all(std::vector<size_t> ids) = 0;
  virtual Status clear() = 0;

  virtual Vector get(size_t id) const = 0;

  virtual StatusOr<size_t> nearest_neighbor(Vector q) const = 0;
  virtual StatusOr<std::vector<size_t>> n_nearest_neighbor(Vector q,
                                                           int n) const = 0;
  virtual Status n_nearest_neighbor(Vector q, int n,
                                    std::vector<size_t> ids) const = 0;
  virtual StatusOr<std::vector<size_t>> search_prism(Vector b,
                                                     Vector s) const = 0;
  virtual Status search_prism(Vector b, Vector s,
                              std::vector<size_t> ids) const = 0;
  virtual StatusOr<std::vector<size_t>> search_ball(Vector b,
                                                    float r) const = 0;
  virtual Status search_ball(Vector b, float r,
                             std::vector<size_t> ids) const = 0;

private:
  int d_;
};

class SimpleSpatialContainer : public SpatialContainer {
public:
  SimpleSpatialContainer(int d) : SpatialContainer(d) {}

  size_t size() const override;
  std::pair<Vector, Vector> bounds() const override;
  StatusOr<std::pair<Vector, Vector>> range() const override;

  size_t add(Vector p) override;
  std::vector<size_t> add_all(std::vector<Vector> ps) override;
  Status remove(size_t id) override;
  Status remove_all(std::vector<size_t> ids) override;
  Status clear() override;

  Vector get(size_t id) const override;

  StatusOr<size_t> nearest_neighbor(Vector q) const override;
  StatusOr<std::vector<size_t>> n_nearest_neighbor(Vector q,
                                                   int n) const override;
  Status n_nearest_neighbor(Vector q, int n,
                            std::vector<size_t> ids) const override;
  StatusOr<std::vector<size_t>> search_prism(Vector b, Vector s) const override;
  Status search_prism(Vector b, Vector s,
                      std::vector<size_t> ids) const override;
  StatusOr<std::vector<size_t>> search_ball(Vector b, float r) const override;
  Status search_ball(Vector b, float r, std::vector<size_t> ids) const override;

private:
  std::map<Vector, size_t> points_;
};

} // namespace spatial
} // namespace container

#endif // CONTAINER_SPATIAL_SPATIAL_CONTAINER_H