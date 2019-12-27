#ifndef GRID_H
#define GRID_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <optional>

#include "color.h"

template<typename T, typename C>
class Grid;

template<typename T, typename C>
Grid<T, C> readBin(const char *filename);

template<typename T, typename C>
class Grid {
  T *data;
  C getter;
 public:
  const int rows, cols, dim;
  Grid(int _rows, int _cols, int _dim) : rows(_rows), cols(_cols), dim(_dim), data(0) {}
  ~Grid();
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
  void savePpm(const char *fn) const;
  void saveBin(const char *fn) const;
  void saveTxt(const char *fn) const;
  void print() const;
  friend Grid readBin<T, C>(const char *filename);
  //template<typename T1, typename T2>
  //friend Grid combine<T, T1, T2, C>(const char *filename1, const char *filename2, int dim, void (*combiner)(const T1 *a, const T2 *b, T *c));
};

template<typename T, typename C>
Grid<T, C> readBin(const char *filename) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  int rows, cols, dim;
  /*
  char *d = (char *) &rows;
  for(int i = 0; i < sizeof(int); i++) {
    ifs >> d[i];
  }
  d = (char *) &cols;
  for(int i = 0; i < sizeof(int); i++) {
    ifs >> d[i];
  }
  //*/
  ifs.read((char *) &rows, sizeof(int));
  ifs.read((char *) &cols, sizeof(int));
  ifs.read((char *) &dim, sizeof(int));
  Grid<T, C> grid(rows, cols, dim);
  grid.allocate();
  ifs.read((char *) grid.data, rows * cols * dim * sizeof(T));
  ifs.close();
  return grid;
}

template<typename T, typename C>
bool Grid<T, C>::readTxt(const char *filename) {
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
Grid<T, C>::~Grid() {
  if(data != 0) {
    delete[] data;
  }
}

template<typename T, typename C>
void Grid<T, C>::allocate() {
  if(data != 0) {
    return;
  }
  data = new T[rows * cols * dim];
}

template<typename T, typename C>
T& Grid<T, C>::getFirst(int i, int j) {
  return data[(i * cols + j) * dim];
}

template<typename T, typename C>
const T& Grid<T, C>::getFirst(int i, int j) const {
  return *data[i * cols + j];
}

template<typename T, typename C>
T *Grid<T, C>::get(int i, int j) {
  return &data[(i * cols + j) * dim];
}

template<typename T, typename C>
const T *Grid<T, C>::get(int i, int j) const {
  int index = (i * cols + j) * dim;
  //std::cout << "get(" << i << ", " << j << ") -> &data[" << index << "]" << std::endl;
  return &data[index];
}

template<typename T, typename C>
T& Grid<T, C>::get(int i, int j, int k) {
  return data[(i * cols + j) * dim + k];
}

template<typename T, typename C>
T& Grid<T, C>::setFirst(int i, int j, T v) {
  data[i * cols + j] = v;
  return data[(i * cols + j) * dim];
}

template<typename T, typename C>
T *Grid<T, C>::set(int i, int j, T *v) {
  for(int k = 0; k < dim; k++) {
    data[(i * cols + j) * dim + k] = v[k];
  }
  return &data[(i * cols + j) * dim];
}

template<typename T, typename C>
T *Grid<T, C>::set(int i, int j, int k, T v) {
  int index = (i * cols + j) * dim + k;
  /*
  std::cout << "set(" << i << ", " << j << ", " << k << ", ";
  getter.print(v, std::cout);
  std::cout << ") -> &data[" << index << "]" << std::endl;
  //*/
  data[index] = v;
  return &data[index];
}

template<typename T, typename C>
void Grid<T, C>::savePpm(const char *filename) const {
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  ofs << "P6\n" << cols << " " << rows << "\n255\n";
  for (unsigned i = 0; i < rows; ++i)
    for (unsigned j = 0; j < cols; ++j) {
      const T *v = get(i, j);
      ofs << (unsigned char)(std::max(float(0), std::min(float(1), getter.r(v, dim))) * 255) <<
             (unsigned char)(std::max(float(0), std::min(float(1), getter.g(v, dim))) * 255) <<
             (unsigned char)(std::max(float(0), std::min(float(1), getter.b(v, dim))) * 255);
    }
  ofs.close();
}

template<typename T, typename C>
void Grid<T, C>::saveBin(const char *filename) const {
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  char *d = (char *) &rows;
  for(int i = 0; i < sizeof(int); i++) {
    ofs << d[i];
  }
  d = (char *) &cols;
  for(int i = 0; i < sizeof(int); i++) {
    ofs << d[i];
  }
  d = (char *) &dim;
  for(int i = 0; i < sizeof(int); i++) {
    ofs << d[i];
  }
  ofs.write((char *) data, rows * cols * dim * sizeof(T));
  ofs.flush();
  ofs.close();
}

template<typename T, typename C>
void Grid<T, C>::saveTxt(const char *filename) const {
  std::ofstream ofs(filename, std::ios::out);
  ofs << cols << " " << rows << std::endl;
  for (unsigned i = 0; i < rows; ++i) {
    for (unsigned j = 0; j < cols; ++j) {
      getter.print(data[i * cols + j], ofs);
      std::cout << " ";
    }
    std::cout << std::endl;
  }
  ofs.close();
}

template<typename T, typename C>
void Grid<T, C>::print() const {
  for (unsigned i = 0; i < rows; i++) {
    std::cout << "Row " << i << ": ";
    for (unsigned j = 0; j < cols; j++) {
      const T *v = get(i, j);
      std::cout << "Col " << j << ": ";
      std::cout << "(" <<
             (int)(std::max(float(0), std::min(float(1), getter.r(v, dim))) * 255) << "," <<
             (int)(std::max(float(0), std::min(float(1), getter.g(v, dim))) * 255) << "," <<
             (int)(std::max(float(0), std::min(float(1), getter.b(v, dim))) * 255) << ") ";
    }
    std::cout << std::endl;
  }
}

class GrayscaleComponent {
 public:
  float r(const float *c, int dim) const {
    return *c;
  }

  float g(const float *c, int dim) const {
    return *c;
  }

  float b(const float *c, int dim) const {
    return *c;
  }

  void print(const float *c, int dim, std::ostream& os) {
    os << *c;
  }

  void parse(float *c, int dim, std::istream& is) {
    float _c;
    is >> _c;
    *c = _c;
  }
};

class SignedGrayscaleComponent {
 public:
  float r(const float *c, int dim) const {
    return *c * 0.5 + 0.5;
  }

  float g(const float *c, int dim) const {
    return *c * 0.5 + 0.5;
  }

  float b(const float *c, int dim) const {
    return *c * 0.5 + 0.5;
  }

  void print(const float& c, std::ostream& os) {
    os << c * 0.5 + 0.5;
  }
};

class ArrayComponent {
 public:
  float r(const float *c, int dim) const {
    return c[0];
  }

  float g(const float *c, int dim) const {
    return dim > 1 ? c[1] : 0.0;
  }

  float b(const float *c, int dim) const {
    return dim > 2 ? c[2] : 0.0;
  }

  void print(const float& c, std::ostream& os) {
    os << c;
  }
};

class NormalComponent {
 public:
  float r(const float *c, int dim) const {
    return c[0] * 0.5 + 0.5;
  }

  float g(const float *c, int dim) const {
    return dim > 1 ? c[1] * 0.5 + 0.5 : 0.0;
  }

  float b(const float *c, int dim) const {
    return dim > 2 ? c[2] * 0.5 + 0.5 : 0.0;
  }

  void print(const float& c, std::ostream& os) {
    os << c * 0.5 + 0.5;
  }
};

template<size_t d>
class StandardArrayComponent {
 public:
  float r(const std::array<float, d>& c) const {
    return c[0];
  }

  float g(const std::array<float, d>& c) const {
    return d > 1 ? c[1] : 0.0;
  }

  float b(const std::array<float, d>& c) const {
    return d > 2 ? c[2] : 0.0;
  }
};

class ColorComponent {
 public:
  float r(const Color *c, int dim) const {
    return c->r();
  }
  float g(const Color *c, int dim) const {
    return c->g();
  }
  float b(const Color *c, int dim) const {
    return c->b();
  }
};

#endif // GRID_H