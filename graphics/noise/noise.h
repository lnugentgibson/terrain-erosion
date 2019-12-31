#ifndef NOISE_H
#define NOISE_H

#include "graphics/image/binary/binimg.h"

float curve1(float x);

float curve3(float x);

float curve(float x, int d);

struct PerlinState {
  float *row_a;
  float *row_b;
  int cur;
  int row;
  float min_val;
  float max_val;
};

class PerlinValueGenerator : public graphics::image::binary::Generator {
  int cell_size;
 public:
  PerlinValueGenerator(int cs) : cell_size(cs) {}
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size);
  
  void GenerateStateful(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, void *state) override;
};

class PerlinGradientGenerator : public graphics::image::binary::Generator {
  int cell_size;
 public:
  PerlinGradientGenerator(int cs) : cell_size(cs) {}
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size);
  
  void GenerateStateful(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, void *state) override;
};

#endif // NOISE_H