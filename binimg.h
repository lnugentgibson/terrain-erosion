#ifndef BINIMG_H
#define BINIMG_H

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

#endif // BINIMG_H