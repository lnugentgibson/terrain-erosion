#ifndef NOISE_H
#define NOISE_H

#include "graphics/image/binary/binimg.h"

using graphics::image::binary::Generator;
using graphics::image::binary::GeneratorBuilder;

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

class PerlinValueGenerator : public Generator {
  int cell_size;
 public:
  PerlinValueGenerator(int cs) : cell_size(cs) {}
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size);
  
  void GenerateStateful(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, void *state) override;
};
  
class PerlinValueGeneratorBuilder : public GeneratorBuilder {
  int cell_size;
 public:
  std::unique_ptr<Generator> operator ()() override {
    return std::unique_ptr<Generator>(new PerlinValueGenerator(cell_size));
  }
  bool SetIntParam(const std::string& param, int value) override;
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

class PerlinGradientGenerator : public Generator {
  int cell_size;
 public:
  PerlinGradientGenerator(int cs) : cell_size(cs) {}
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size);
  
  void GenerateStateful(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, void *state) override;
};
  
class PerlinGradientGeneratorBuilder : public GeneratorBuilder {
  int cell_size;
 public:
  std::unique_ptr<Generator> operator ()() override {
    return std::unique_ptr<Generator>(new PerlinGradientGenerator(cell_size));
  }
  bool SetIntParam(const std::string& param, int value) override;
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

#endif // NOISE_H