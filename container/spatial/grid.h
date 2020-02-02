#ifndef CONTAINER_SPATIAL_GRID_H
#define CONTAINER_SPATIAL_GRID_H

#include <iostream>
#include <list>
#include <vector>

namespace container {
namespace spatial {
  
template<typename T>
class GridCell {
  friend Grid<T>;
 private:
  std::map<Vector, T> contents;
};
    
template<typename T>
class Grid {
  template<int d>
  Grid(int sizes[d]) : dim(d) {
    int s = 1;
    for(int i = 0; i < d; i++) {
      size.push_back(sizes[i]);
      s *= sizes[i];
    }
    for(int i = 0; i < s; i++) {
      grid.emplace_back();
    }
  }
 private:
  std::vector<int> size;
  std::list<GridCell<T>> grid;
};

} // namespace spatial
} // namespace container

#endif // CONTAINER_SPATIAL_GRID_H