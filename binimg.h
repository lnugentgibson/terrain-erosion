#ifndef BINIMG_H
#define BINIMG_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>

template<typename T, typename T1, typename T2, typename C>
void combineBin(const char *filename1, const char *filename2, const char *filename, int dim, C combiner) {
  std::ifstream ifs1(filename1, std::ios::in | std::ios::binary);
  std::ifstream ifs2(filename2, std::ios::in | std::ios::binary);
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  int rows, cols, dim1;
  int _rows, _cols, dim2;
  ifs1.read((char *) &rows, sizeof(int));
  ifs1.read((char *) &cols, sizeof(int));
  ifs1.read((char *) &dim1, sizeof(int));
  ifs2.read((char *) &_rows, sizeof(int));
  ifs2.read((char *) &_cols, sizeof(int));
  ifs2.read((char *) &dim2, sizeof(int));
  std::cout << "rows 1: " << rows << std::endl;
  std::cout << "cols 1: " << cols << std::endl;
  std::cout << "dim 1: " << dim1 << std::endl;
  std::cout << "rows 2: " << _rows << std::endl;
  std::cout << "cols 2: " << _cols << std::endl;
  std::cout << "dim 2: " << dim2 << std::endl;
  ofs.write((char *) &rows, sizeof(int));
  ofs.write((char *) &cols, sizeof(int));
  ofs.write((char *) &dim, sizeof(int));
  if(rows != _rows) {
    return;
  }
  if(cols != _cols) {
    return;
  }
  T *pixel = new T[dim];
  T1 *pixel1 = new T1[dim1];
  T2 *pixel2 = new T2[dim2];
  for(int i = 0; i < rows; i++) {
    //std::cout << "row " << i + 1 << std::endl;
    for(int j = 0; j < cols; j++) {
      //std::cout << "\tcol " << j + 1 << std::endl;
      ifs1.read((char *) pixel1, dim1 * sizeof(T1));
      //std::cout << "\t\tpixel1: ";
      for(int k = 0; k < dim1; k++) {
        //std::cout << pixel1[k] << (k + 1 < dim1 ? ", " : " ");
      }
      //std::cout << std::endl;
      ifs2.read((char *) pixel2, dim2 * sizeof(T2));
      //std::cout << "\t\tpixel2: ";
      for(int k = 0; k < dim1; k++) {
        //std::cout << pixel2[k] << (k + 1 < dim2 ? ", " : " ");
      }
      //std::cout << std::endl;
      combiner(pixel1, pixel2, pixel);
      //std::cout << "\t\tpixel: ";
      for(int k = 0; k < dim; k++) {
        //std::cout << pixel[k] << (k + 1 < dim ? ", " : " ");
      }
      //std::cout << std::endl;
      ofs.write((char *) pixel, dim * sizeof(T));
    }
  }
  ifs1.close();
  ifs2.close();
  ofs.close();
}

template<typename T, typename S, typename E>
S summarizeBin(const char *filename, E evaluator) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  int rows, cols, dim;
  ifs.read((char *) &rows, sizeof(int));
  ifs.read((char *) &cols, sizeof(int));
  ifs.read((char *) &dim, sizeof(int));
  std::cout << "rows: " << rows << std::endl;
  std::cout << "cols: " << cols << std::endl;
  std::cout << "dim: " << dim << std::endl;
  S summary;
  T *pixel = new T[dim];
  for(int i = 0; i < rows * cols; i++) {
    ifs.read((char *) pixel, dim * sizeof(T));
    evaluator(pixel, dim, i, &summary);
  }
  ifs.close();
  return summary;
}

template<typename T1, typename T2, typename T>
void transformBin(const char *filename1, const char *filename2, int dim2, T transformer) {
  std::ifstream ifs(filename1, std::ios::in | std::ios::binary);
  std::ofstream ofs(filename2, std::ios::out | std::ios::binary);
  int rows, cols, dim1;
  ifs.read((char *) &rows, sizeof(int));
  ifs.read((char *) &cols, sizeof(int));
  ifs.read((char *) &dim1, sizeof(int));
  std::cout << "rows: " << rows << std::endl;
  std::cout << "cols: " << cols << std::endl;
  std::cout << "dim: " << dim1 << std::endl;
  ofs.write((char *) &rows, sizeof(int));
  ofs.write((char *) &cols, sizeof(int));
  ofs.write((char *) &dim2, sizeof(int));
  T1 *pixel1 = new T1[dim1];
  T2 *pixel2 = new T2[dim2];
  for(int i = 0; i < rows * cols; i++) {
    ifs.read((char *) pixel1, dim1 * sizeof(T1));
    transformer(pixel1, dim1, pixel2, dim2);
    ofs.write((char *) pixel2, dim2 * sizeof(T2));
  }
  ifs.close();
  ofs.close();
}

template<typename T1, typename T2, typename T>
void transformBinNeighbors(const char *filename1, const char *filename2, int dim2, int w, T transformer);

template<typename T1>
class Neighborhood {
  T1 *data;
  int start, end, center_i, center_j;
  //*
  void check() {
    while(start < 0 || center_i < 0 || end < 0) {
      start += rows;
      center_i += rows;
      end += rows;
    }
    while(start >= rows && center_i >= rows && end >= rows) {
      start -= rows;
      center_i -= rows;
      end -= rows;
    }
  }
  T1 *addRow() {
    end++;
    check();
    return data + ((end + rows - 1) % rows);
  }
  void removeRow() {
    start++;
    check();
  }
  //*/
 public:
  const int rows, cols, dim;
  Neighborhood(int _rows, int _cols, int _dim) : rows(_rows), cols(_cols), dim(_dim), data(new T1[rows * cols * dim]), start(0), end(0), center_i(0), center_j(0) {}
  ~Neighborhood() {
    delete[] data;
  }
  //*
  std::optional<T1> get(int i, int j) const {
    if(center_i + i < start || center_i + i >= end) {
      return std::nullopt;
    }
    if(center_j + j < 0 || center_j + j >= cols) {
      return std::nullopt;
    }
    return data[((center_i + i) % rows) * cols + center_j + j];
  }
  std::array<float,4> range() const {
    return {start - center_i, end - center_i, -std::min(center_j, rows), std::min(cols - center_j, rows)};
  }
  //*/
  template<typename U1, typename U2, typename U>
  friend void transformBinNeighbors<U1, U2, U>(const char *filename1, const char *filename2, int dim2, int w, U transformer);
};

template<typename T1, typename T2, typename T>
void transformBinNeighbors(const char *filename1, const char *filename2, int dim2, int w, T transformer) {
  std::ifstream ifs(filename1, std::ios::in | std::ios::binary);
  std::ofstream ofs(filename2, std::ios::out | std::ios::binary);
  int rows, cols, dim1;
  ifs.read((char *) &rows, sizeof(int));
  ifs.read((char *) &cols, sizeof(int));
  ifs.read((char *) &dim1, sizeof(int));
  std::cout << "rows: " << rows << std::endl;
  std::cout << "cols: " << cols << std::endl;
  std::cout << "dim: " << dim1 << std::endl;
  ofs.write((char *) &rows, sizeof(int));
  ofs.write((char *) &cols, sizeof(int));
  ofs.write((char *) &dim2, sizeof(int));
  int W = 2 * w + 1;
  Neighborhood<T1> neighborhood(W, cols, dim1);
  T2 *pixels2 = new T2[dim2 * W];
  for(int i = 0; i < w; i++) {
    //ifs.read((char *) neighborhood.addRow(), dim1 * cols * sizeof(T1));
  }
  for(int i = 0; i < rows; i++) {
    if(i + w < rows) {
      //ifs.read((char *) neighborhood.addRow(), dim1 * cols * sizeof(T1));
    }
    if(i > w) {
      //neighborhood.removeRow();
    }
    neighborhood.center_j = 0;
    for(int j = 0; j < cols; j++) {
      transformer(rows, cols, &neighborhood, dim1, pixels2, dim2);
      ofs.write((char *) pixels2, dim2 * sizeof(T2));
      neighborhood.center_j++;
    }
    neighborhood.center_i++;
  }
  ifs.close();
  ofs.close();
}

#endif // BINIMG_H