#ifndef CONTAINER_SPATIAL_GRID_H
#define CONTAINER_SPATIAL_GRID_H

#include <iostream>
#include <list>
#include <map>
#include <vector>

#include "math/vector/vector.h"
#include "util/status.h"

using math::vector::Vector;
using math::vector::IVector;

namespace container {
namespace spatial {

template <typename T> class Grid;

template <typename T> class GridCell {
  friend Grid<T>;

private:
  std::map<Vector, T> contents;
};

template <typename T> class Grid {
public:
  Grid(int d, int sizes[], Vector b, Vector r, Vector c)
      : base(b), range(r), cell_size(c) {
    int s = 1;
    for (int i = 0; i < d; i++) {
      size.push_back(sizes[i]);
      s *= sizes[i];
    }
    for (int i = 0; i < s; i++) {
      grid.emplace_back();
    }
  }

  util::Status Push(Vector p, T elem) {
    Vector q = p / cell_size;
    IVector c = q.floor();
    Vector r = p - (Vector(c) * cell_size);
    return util::OK;
  }

private:
  std::vector<int> size;
  std::list<GridCell<T>> grid;
  Vector base, range, cell_size;
};

} // namespace spatial
} // namespace container

#endif // CONTAINER_SPATIAL_GRID_H