#ifndef CONTAINER_SPATIAL_GRID_H
#define CONTAINER_SPATIAL_GRID_H

#include <cassert>
#include <deque>
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

struct GridElement {
  IVector cell;
  Vector offset;
  GridElement() : cell(2), offset(2) {}
  GridElement(int d) : cell(d), offset(d) {}
  GridElement(IVector i, Vector v) : cell(i), offset(v) {}
};

class Grid {
public:
  Grid(int d, int sizes[], Vector b, Vector r, Vector c)
      : base(b), range(r), cell_size(c) {
    divisions.reserve(d);
    for (int i = 0; i < d; i++) {
      divisions.push_back(sizes[i]);
    }
  }

  bool empty() { return cells.empty(); }
  size_t size() { return cells.size(); }

  void clear() { cells.clear(); }
  size_t insert(const Vector &p) {
    size_t index = 0;
    Vector quotient = (p - base) / cell_size;
    IVector cell = quotient.floor();
    Vector offset = (p - (Vector(cell) * cell_size)) / cell_size;
    if (free_indices.empty()) {
      index = elements.size();
      elements.emplace_back(cell, offset);
    } else {
      index = free_indices.front();
      elements[index] = GridElement(cell, offset);
      free_indices.pop_front();
    }
    size_t cell_index = GetIndex(cell);
    cells.insert({cell_index, index});
    return index;
  }
  void erase(size_t id);
  void erase(Vector p);
  void set(size_t id, const Vector &p);

private:
  size_t GetIndex(const IVector &c) const {
    assert(c.valid());
    size_t p = 0;
    int f = 1;
    for (int i = divisions.size() - 1; i >= 0; i--) {
      p += f * c[i];
      f *= divisions[i];
    }
    return p;
  }
  IVector GetPosition(int p) const {
    IVector c(divisions.size());
    for (size_t i = 0; i < divisions.size(); i++) {
      c[i] = p % divisions[i];
      p /= divisions[i];
    }
    return c;
  }

  std::vector<int> divisions;
  std::vector<GridElement> elements;
  std::multimap<size_t, size_t> cells;
  std::deque<size_t> free_indices;
  Vector base, range, cell_size;
};

} // namespace spatial
} // namespace container

#endif // CONTAINER_SPATIAL_GRID_H