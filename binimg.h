#ifndef BINIMG_H
#define BINIMG_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>

template<typename G>
void generateBin(int rows, int cols, int dim, size_t element_size, std::ostream& os, G generator) {
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      generator(i, j, rows, cols, pixel, dim, element_size);
      os.write((char *) pixel, dim * element_size);
    }
}

template<typename S, typename G>
void generateStatefulBin(int rows, int cols, int dim, size_t element_size, std::ostream& os, G generator) {
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  S state;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      generator(i, j, rows, cols, pixel, dim, element_size, &state);
      os.write((char *) pixel, dim * element_size);
    }
}

template<typename S, typename G>
void generateStatefulBin(int rows, int cols, int dim, size_t element_size, std::ostream& os, G generator, S *init) {
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  S *state = init == 0 ? new S() : init;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      generator(i, j, rows, cols, pixel, dim, element_size, state);
      os.write((char *) pixel, dim * element_size);
    }
  if(init == 0) delete state;
}

template<typename F>
void forEachBin(size_t element_size, std::istream& is, F func) {
  int rows, cols, dim;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel, dim * element_size);
      func(i, j, rows, cols, pixel, dim, element_size);
    }
}

template<typename S, typename F>
void forEachStatefulBin(size_t element_size, std::istream& is, F func) {
  int rows, cols, dim;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  S state;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel, dim * element_size);
      func(i, j, rows, cols, pixel, dim, element_size, &state);
    }
}

template<typename M>
void mapBin(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, M map) {
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
      map(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2);
      os.write((char *) pixel2, dim2 * element_size2);
    }
}

template<typename S, typename M>
void mapStatefulBin(size_t element_size1, std::istream& is, size_t element_size2, int dim2, std::ostream& os, M map) {
  int rows, cols, dim1;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim1, sizeof(int));
  os.write((char *) &rows, sizeof(int));
  os.write((char *) &cols, sizeof(int));
  os.write((char *) &dim2, sizeof(int));
  char *pixel1 = new char[dim1 * element_size1];
  char *pixel2 = new char[dim2 * element_size2];
  S state;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel1, dim1 * element_size1);
      map(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2, &state);
      os.write((char *) pixel2, dim2 * element_size2);
    }
}

template<typename R, typename A>
A reduceBin(size_t element_size, std::istream& is, R reducer) {
  int rows, cols, dim;
  is.read((char *) &rows, sizeof(int));
  is.read((char *) &cols, sizeof(int));
  is.read((char *) &dim, sizeof(int));
  char *pixel = new char[dim * element_size];
  A aggregate;
  int n = 0;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is.read((char *) pixel, dim * element_size);
      reducer(i, j, rows, cols, pixel, dim, element_size, n, aggregate);
      n++;
    }
  return aggregate;
}

template<typename C>
void combineBin(size_t element_size1, std::istream& is1, size_t element_size2, std::istream& is2, size_t element_size3, int dim3, std::ostream& os, C combiner) {
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
      combiner(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2, pixel3, dim3, element_size3);
      os.write((char *) pixel3, dim3 * element_size3);
    }
}

template<typename S, typename C>
void combineStatefulBin(size_t element_size1, std::istream& is1, size_t element_size2, std::istream& is2, size_t element_size3, int dim3, std::ostream& os, C combiner) {
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
  S state;
  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++) {
      is1.read((char *) pixel1, dim1 * element_size1);
      is2.read((char *) pixel2, dim2 * element_size2);
      combiner(i, j, rows, cols, pixel1, dim1, element_size1, pixel2, dim2, element_size2, pixel3, dim3, element_size3, &state);
      os.write((char *) pixel3, dim3 * element_size3);
    }
}

template<typename C>
void ppmBin(size_t element_size, std::istream& is, std::ostream& os, C component) {
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
      component(pixel, dim, element_size, rgb);
      os << (unsigned char)(std::max(float(0), std::min(float(1), rgb[0])) * 255) <<
             (unsigned char)(std::max(float(0), std::min(float(1), rgb[1])) * 255) <<
             (unsigned char)(std::max(float(0), std::min(float(1), rgb[2])) * 255);
    }
}

#endif // BINIMG_H