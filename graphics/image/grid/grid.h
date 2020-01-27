#ifndef GRAPHICS_IMAGE_GRID_GRID_H
#define GRAPHICS_IMAGE_GRID_GRID_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>

#include "graphics/color/color.h"
#include "graphics/image/pixel.h"

namespace graphics {
namespace image {
namespace grid {

class Grid {
  DataSpecifier spec;
  char *data;
 public:
  const int rows, cols;
  Grid(int r, int c, DataSpecifier d) : rows(r), cols(c), spec(d), data(0) {}
  ~Grid();
  void allocate();
  char *get(int i, int j);
  const char *get(int i, int j) const;
  char *get(int i, int j, int k);
  const char *get(int i, int j, int k) const;
};

template<typename T, typename C>
class TypedGrid;

template<typename T, typename C>
TypedGrid<T, C> readBin(const char *filename);

template<typename T, typename C>
class TypedGrid {
  T *data;
 public:
  const int rows, cols, dim;
  TypedGrid(int _rows, int _cols, int _dim) : rows(_rows), cols(_cols), dim(_dim), data(0) {}
  ~TypedGrid();
  void allocate();
  bool readTxt(const char *filename);
  T& getFirst(int i, int j);
  const T& getFirst(int i, int j) const;
  T *get(int i, int j);
  const T *get(int i, int j) const;
  T& get(int i, int j, int k);
  T& setFirst(int i, int j, T v);
  T *set(int i, int j, T *v);
  T *set(int i, int j, int k, T v);
  //template<typename T1, typename T2>
  //friend TypedGrid combine<T, T1, T2, C>(const char *filename1, const char *filename2, int dim, void (*combiner)(const T1 *a, const T2 *b, T *c));
};

template<typename T, typename C>
bool TypedGrid<T, C>::readTxt(const char *filename) {
  std::ifstream ifs(filename, std::ios::in);
  int _rows, _cols;
  ifs >> _rows;
  ifs >> _cols;
  if(rows != _rows) {
    std::cout << "Invalid row count: " << rows << "(expected) " << _rows << "(actual)" << std::endl;
    return false;
  }
  if(cols != _cols) {
    std::cout << "Invalid col count: " << cols << "(expected) " << _cols << "(actual)" << std::endl;
    return false;
  }
  allocate();
  for(int i = 0; i < rows * cols; i++) {
    getter.parse(data[i], ifs);
  }
  ifs.close();
  return true;
}

template<typename T, typename C>
TypedGrid<T, C>::~TypedGrid() {
  if(data != 0) {
    delete[] data;
  }
}

template<typename T, typename C>
void TypedGrid<T, C>::allocate() {
  if(data != 0) {
    return;
  }
  data = new T[rows * cols * dim];
}

template<typename T, typename C>
T& TypedGrid<T, C>::getFirst(int i, int j) {
  return data[(i * cols + j) * dim];
}

template<typename T, typename C>
const T& TypedGrid<T, C>::getFirst(int i, int j) const {
  return *data[i * cols + j];
}

template<typename T, typename C>
T *TypedGrid<T, C>::get(int i, int j) {
  return &data[(i * cols + j) * dim];
}

template<typename T, typename C>
const T *TypedGrid<T, C>::get(int i, int j) const {
  int index = (i * cols + j) * dim;
  //std::cout << "get(" << i << ", " << j << ") -> &data[" << index << "]" << std::endl;
  return &data[index];
}

template<typename T, typename C>
T& TypedGrid<T, C>::get(int i, int j, int k) {
  return data[(i * cols + j) * dim + k];
}

template<typename T, typename C>
T& TypedGrid<T, C>::setFirst(int i, int j, T v) {
  data[i * cols + j] = v;
  return data[(i * cols + j) * dim];
}

template<typename T, typename C>
T *TypedGrid<T, C>::set(int i, int j, T *v) {
  for(int k = 0; k < dim; k++) {
    data[(i * cols + j) * dim + k] = v[k];
  }
  return &data[(i * cols + j) * dim];
}

template<typename T, typename C>
T *TypedGrid<T, C>::set(int i, int j, int k, T v) {
  int index = (i * cols + j) * dim + k;
  /*
  std::cout << "set(" << i << ", " << j << ", " << k << ", ";
  getter.print(v, std::cout);
  std::cout << ") -> &data[" << index << "]" << std::endl;
  //*/
  data[index] = v;
  return &data[index];
}

} // namespace grid
} // namespace image
} // namespace graphics

#endif // GRAPHICS_IMAGE_GRID_GRID_H