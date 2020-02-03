#ifndef CONTAINER_SPATIAL_GRID_H
#define CONTAINER_SPATIAL_GRID_H

#include <iostream>
#include <map>
#include <optional>
#include <vector>

#include "math/vector/vector.h"
#include "util/status.h"

using math::vector::IVector;
using math::vector::Vector;

namespace container {
namespace spatial {

class Grid;

class GridCell {
  friend Grid;

public:
  void Insert(Vector p, int64_t id) {
  }
  bool InsertIfNotPresent(Vector p, int64_t id) {
    return true;
  }

private:
  std::map<Vector, int64_t> contents;
};

class Grid {
public:
  Grid(int d, int sizes[], Vector b, Vector r, Vector c)
      : base(b), range(r), cell_size(c) {
    int s = 1;
    size.reserve(d);
    for (int i = 0; i < d; i++) {
      size.push_back(sizes[i]);
      s *= sizes[i];
    }
    grid.reserve(s);
    for (int i = 0; i < s; i++) {
      grid.emplace_back();
    }
  }

  int64_t Insert(Vector p) {
    Vector q = p / cell_size;
    IVector c = q.floor();
    Vector r = p - (Vector(c) * cell_size);
    grid[GetIndex(c)].Insert(r / cell_size, next_id);
    return next_id++;
  }
  std::optional<int64_t> InsertIfNotPresent(Vector p) {
    Vector q = p / cell_size;
    IVector c = q.floor();
    Vector r = p - (Vector(c) * cell_size);
    if(grid[GetIndex(c)].InsertIfNotPresent(r / cell_size, next_id)) {
      return next_id++;
    }
    return std::nullopt;
  }

private:
  int GetIndex(IVector c) const {
    int p = 0, f = 1;
    for (int i = size.size() - 1; i >= 0; i--) {
      p += f * c[i];
      f *= size[i];
    }
    return p;
  }

  std::vector<int> size;
  std::vector<GridCell> grid;
  Vector base, range, cell_size;
  int64_t next_id;
};

} // namespace spatial
} // namespace container

#endif // CONTAINER_SPATIAL_GRID_H