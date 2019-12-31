#include "binimg.h"

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

Neighborhood::Neighborhood(std::deque<char *>& _buffer, int _span, int _cols, int _dim, size_t _element_size, int _center_i, int _center_j) :
  buffer(_buffer), span(_span), cols(_cols), dim(_dim), element_size(_element_size), center_i(_center_i), center_j(_center_j) {}

std::array<int, 4> Neighborhood::range() {
  if(buffer.empty()) return {0, 0, 0, 0};
  return std::array<int, 4>({-center_i, (int) buffer.size() - center_i, -std::min(center_j, span), std::min(cols - center_j, span)});
}

char *Neighborhood::get(int i, int j) {
  i += center_i;
  if(i < 0 || i >= (int) buffer.size()) {
    return nullptr;
  }
  j += center_j;
  if(j < 0 || j >= cols) {
    return nullptr;
  }
  char *row = buffer[i];
  char *pixel = new char[dim * element_size];
  std::copy(row + j * dim * element_size, row + (j + 1) * dim * element_size, pixel);
  return pixel;
}

void Generate(int rows, int cols, int dim, size_t element_size, std::ostream& os, Generator *generator) {
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      generator->Generate(i, j, rows, cols, pixel, dim, element_size);
      os.write((char *) pixel, dim * element_size);
    }
}

void *GenerateStateful(int rows, int cols, int dim, size_t element_size, std::ostream& os, Generator *generator, void *initial) {
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  void *state = initial;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      generator->GenerateStateful(i, j, rows, cols, pixel, dim, element_size, state);
      os.write((char *) pixel, dim * element_size);
    }
  return state;
}

void ForEach(size_t element_size, std::istream& is, Functor *functor) {
  int rows, cols, dim;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel, dim * element_size);
      functor->Do(i, j, rows, cols, pixel, dim, element_size);
    }
}

void *ForEachStateful(size_t element_size, std::istream& is, Functor *functor, void *initial) {
  int rows, cols, dim;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  void *state = initial;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel, dim * element_size);
      functor->DoStateful(i, j, rows, cols, pixel, dim, element_size, state);
    }
  return state;
}

void Map(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, Transformer *map) {
  int rows, cols, dim1;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim1, sizeof(int));
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim2, sizeof(int));
  char *pixel1 = new char[dim1 * element_size1];
  char *pixel2 = new char[dim2 * element_size2];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel1, dim1 * element_size1);
      map->Transform(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2);
      os.write((char *) pixel2, dim2 * element_size2);
    }
}

void *MapStateful(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, Transformer *map, void *initial) {
  int rows, cols, dim1;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim1, sizeof(int));
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim2, sizeof(int));
  char *pixel1 = new char[dim1 * element_size1];
  char *pixel2 = new char[dim2 * element_size2];
  void *state = initial;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel1, dim1 * element_size1);
      map->TransformStateful(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2, state);
      os.write((char *) pixel2, dim2 * element_size2);
    }
  return state;
}

void MapNeighborhood(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, int span, Transformer *map) {
  int rows, cols, dim1;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim1, sizeof(int));
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim2, sizeof(int));
  char *pixel2 = new char[dim2 * element_size2];
  std::deque<char *> buffer;
  for(int i = 0; i < span; i++) {
    char *row = new char[dim1 * element_size1 * cols];
    is.read((char *) row, dim1 * element_size1);
    buffer.push_back(row);
  }
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      char *row = 0;
      if(i > span) {
        row = buffer.front();
        buffer.pop_front();
      }
      if(rows - i > span) {
        if(row == 0) row = new char[dim1 * element_size1 * cols];
        is.read((char *) row, dim1 * element_size1);
        buffer.push_back(row);
      } else if(row != 0) {
        delete[] row;
      }
      map->TransformNeighborhood(i, j, rows, cols,
        Neighborhood(buffer, span, cols, dim1, element_size1, std::min(i, span), j),
        pixel2, dim2, element_size2);
      os.write((char *) pixel2, dim2 * element_size2);
    }
  while(!buffer.empty()) {
    char *row = buffer.front();
    buffer.pop_front();
    delete[] row;
  }
}

void *Reduce(size_t element_size, std::istream& is, Accumulator *reducer, void *initial) {
  int rows, cols, dim;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  void *aggregate = initial;
  int n = 0;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel, dim * element_size);
      reducer->Aggregate(i, j, rows, cols, pixel, dim, element_size, n, aggregate);
      n++;
    }
  return aggregate;
}

void Combine(size_t element_size1, std::istream& is1, size_t element_size2, std::istream& is2, size_t element_size3, int dim3, std::ostream& os, Combiner *combiner) {
  int rows, cols, dim1;
  int _rows, _cols, dim2;
  is1.read((char *) &rows, sizeof(int));
  is1.read((char *) &cols, sizeof(int));
  is1.read((char *) &dim1, sizeof(int));
  is2.read((char *) &_rows, sizeof(int));
  is2.read((char *) &_cols, sizeof(int));
  is2.read((char *) &dim2, sizeof(int));
  if(rows != _rows || cols != _cols) {
    return;
  }
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim3, sizeof(int));
  char *pixel1 = new char[dim1 * element_size1];
  char *pixel2 = new char[dim2 * element_size2];
  char *pixel3 = new char[dim3 * element_size3];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is1.read((char *) pixel1, dim1 * element_size1);
      is2.read((char *) pixel2, dim2 * element_size2);
      combiner->Combine(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2, pixel3, dim3, element_size3);
      os.write((char *) pixel3, dim3 * element_size3);
    }
}

void *CombineStatefulBin(size_t element_size1, std::istream& is1, size_t element_size2, std::istream& is2, size_t element_size3, int dim3, std::ostream& os, Combiner *combiner, void *initial) {
  int rows, cols, dim1;
  int _rows, _cols, dim2;
  is1.read((char *) &rows, sizeof(int));
  is1.read((char *) &cols, sizeof(int));
  is1.read((char *) &dim1, sizeof(int));
  is2.read((char *) &_rows, sizeof(int));
  is2.read((char *) &_cols, sizeof(int));
  is2.read((char *) &dim2, sizeof(int));
  if(rows != _rows || cols != _cols) {
    return 0;
  }
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim3, sizeof(int));
  char *pixel1 = new char[dim1 * element_size1];
  char *pixel2 = new char[dim2 * element_size2];
  char *pixel3 = new char[dim3 * element_size3];
  void *state = initial;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is1.read((char *) pixel1, dim1 * element_size1);
      is2.read((char *) pixel2, dim2 * element_size2);
      combiner->CombineStateful(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2, pixel3, dim3, element_size3, state);
      os.write((char *) pixel3, dim3 * element_size3);
    }
  return state;
}

void ToPPM(size_t element_size, std::istream& is, std::ostream& os, Colorizer *component) {
  int rows, cols, dim;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim, sizeof(int));
  os << "P6\n" << cols << " " << rows << "\n255\n";
  char *pixel = new char[dim * element_size];
  float *rgb = new float[3];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel, dim * element_size);
      component->ToRGB(pixel, dim, element_size, rgb);
      os << (unsigned char)(std::max(float(0), std::min(float(1), rgb[0])) * 255) <<
             (unsigned char)(std::max(float(0), std::min(float(1), rgb[1])) * 255) <<
             (unsigned char)(std::max(float(0), std::min(float(1), rgb[2])) * 255);
    }
}

} // namespace binary
} // namespace image
} // namespace graphics