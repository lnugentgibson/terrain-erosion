#ifndef BINIMG_H
#define BINIMG_H

#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>

namespace graphics {
namespace image {
namespace binary {

class Neighborhood {
  std::deque<char *>& buffer;
  int span;
  int cols;
  int dim;
  size_t element_size;
  int center_i, center_j;
 public:
  Neighborhood(std::deque<char *>& _buffer, int _span, int _cols, int _dim, size_t _element_size, int _center_i, int _center_j);

  std::array<int, 4> range();
  char* get(int i, int j);
};

class Generator {
 public:
  virtual void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) = 0;
  
  virtual void GenerateStateful(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, void *state) = 0;
};

class StatelessGenerator : public Generator {
 public:
  virtual void GenerateStateful(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, void *state) override {
    this->Generate(i, j, rows, cols, pixel, dim, element_size);
  }
};

void Generate(int rows, int cols, int dim, size_t element_size, std::ostream& os, Generator *generator);

void *GenerateStateful(int rows, int cols, int dim, size_t element_size, std::ostream& os, Generator *generator, void *initial);

class Functor {
 public:
  virtual void Do(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size) = 0;
  
  virtual void DoStateful(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size, void *state) = 0;
};

void ForEach(size_t element_size, std::istream& is, Functor *functor);

void *ForEachStateful(size_t element_size, std::istream& is, Functor *func, void *initial);

class Transformer {
 public:
  virtual void Transform(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2) = 0;
  
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2,
    void *state) = 0;
  
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    void *pixel2, int dim2, size_t element_size2) = 0;
};

void mapBin(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, Transformer *map);

void *mapStatefulBin(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, Transformer *map);

void mapNeighborhoodBin(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, int span, Transformer *map);

class Accumulator {
 public:
  virtual void Aggregate(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size, int n, void *aggregate) = 0;
};

void *Reduce(size_t element_size, std::istream& is, Accumulator *reducer, void *initial);

class Combiner {
 public:
  virtual void Combine(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    const void *pixel2, int dim2, size_t element_size2,
    void *pixel3, int dim3, size_t element_size3) = 0;
    
  virtual void CombineStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    const void *pixel2, int dim2, size_t element_size2,
    void *pixel3, int dim3, size_t element_size3,
    void *state) = 0;
};

void CombineBin(size_t element_size1, std::istream& is1, size_t element_size2, std::istream& is2, size_t element_size3, int dim3, std::ostream& os, Combiner *combiner);

void *CombineStateful(size_t element_size1, std::istream& is1, size_t element_size2, std::istream& is2, size_t element_size3, int dim3, std::ostream& os, Combiner *combiner);

class Colorizer {
 public:
  virtual void ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) = 0;
};

void ToPPM(size_t element_size, std::istream& is, std::ostream& os, Colorizer *component);

} // namespace binary
} // namespace image
} // namespace graphics

#endif // BINIMG_H