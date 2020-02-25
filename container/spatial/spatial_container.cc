#include "container/spatial/spatial_container.h"

namespace container {
namespace spatial {

size_t SimpleSpatialContainer::size() const { return 0; }
// std::pair<Vector, Vector> SimpleSpatialContainer::bounds() const;
// StatusOr<std::pair<Vector, Vector>> SimpleSpatialContainer::range() const;

size_t SimpleSpatialContainer::add(Vector p) { return 0; }
// std::vector<size_t> SimpleSpatialContainer::add_all(std::vector<Vector> ps);
Status SimpleSpatialContainer::remove(size_t id) { return util::OK; }
Status SimpleSpatialContainer::remove_all(std::vector<size_t> ids) { return util::OK; }
Status SimpleSpatialContainer::clear() { return util::OK; }

// Vector SimpleSpatialContainer::get(size_t id) const;

/*
StatusOr<size_t> SimpleSpatialContainer::nearest_neighbor(Vector q) const;
StatusOr<std::vector<size_t>>
SimpleSpatialContainer::n_nearest_neighbor(Vector q, int n) const;
Status
SimpleSpatialContainer::n_nearest_neighbor(Vector q, int n,
                                           std::vector<size_t> ids) const;
StatusOr<std::vector<size_t>>
SimpleSpatialContainer::search_prism(Vector b, Vector s) const;
Status SimpleSpatialContainer::search_prism(Vector b, Vector s,
                                            std::vector<size_t> ids) const;
StatusOr<std::vector<size_t>>
SimpleSpatialContainer::search_ball(Vector b, float r) const;
Status SimpleSpatialContainer::search_ball(Vector b, float r,
                                           std::vector<size_t> ids) const;
*/

} // namespace spatial
} // namespace container