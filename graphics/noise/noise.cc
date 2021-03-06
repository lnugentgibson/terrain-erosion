#include "graphics/noise/noise.h"

#include <cmath>

#include "graphics/image/binary/binimg.h"
#include "graphics/image/binary/binimg_registration.h"
#include "graphics/noise/random.h"

using graphics::image::PixelSpecifier;
using graphics::image::binary::Generator;
using graphics::image::binary::GeneratorBuilder;

float curve1(float x) { return x; }

float curve3(float x) { return x * x * (-x * 2 + 3); }

float curve(float x, int d) {
  switch (d) {
  case 3:
    return x * x * (-x * 2 + 3);
  default:
    return x;
  }
}

class PerlinValueGenerator : public Generator {
  int cell_size;

public:
  PerlinValueGenerator(int cs) : cell_size(cs) {}
  void Generate(int i, int j, int rows, int cols, PixelSpecifier pixel);

  void GenerateStateful(int i, int j, int rows, int cols, PixelSpecifier pixel,
                        void *state) override;
};

class PerlinValueGeneratorBuilder : public GeneratorBuilder {
  int cell_size;

public:
  std::unique_ptr<Generator> operator()() override {
    return std::unique_ptr<Generator>(new PerlinValueGenerator(cell_size));
  }
  bool SetIntParam(const std::string &param, int value) override;
  bool SetFloatParam(const std::string &param, float value) override {
    return false;
  }
};

class PerlinGradientGenerator : public Generator {
  int cell_size;

public:
  PerlinGradientGenerator(int cs) : cell_size(cs) {}
  void Generate(int i, int j, int rows, int cols, PixelSpecifier pixel);

  void GenerateStateful(int i, int j, int rows, int cols, PixelSpecifier pixel,
                        void *state) override;
};

class PerlinGradientGeneratorBuilder : public GeneratorBuilder {
  int cell_size;

public:
  std::unique_ptr<Generator> operator()() override {
    return std::unique_ptr<Generator>(new PerlinGradientGenerator(cell_size));
  }
  bool SetIntParam(const std::string &param, int value) override;
  bool SetFloatParam(const std::string &param, float value) override {
    return false;
  }
};

void PerlinValueGenerator::Generate(int i, int j, int rows, int cols,
                                    PixelSpecifier pixel) {}

void PerlinValueGenerator::GenerateStateful(int i, int j, int rows, int cols,
                                            PixelSpecifier pixel, void *state) {
  PerlinState *perlin_state = static_cast<PerlinState *>(state);
  int row = static_cast<int>(floor(i / cell_size));
  int col = static_cast<int>(floor(j / cell_size));
  int row_size = static_cast<int>(ceil(cols / cell_size + 1));
  if (j == 0 && row != perlin_state->row) {
    float *row =
        perlin_state->cur == 0 ? perlin_state->row_a : perlin_state->row_b;
    for (int i = 0; i < row_size; i++) {
      float v = randGray();
      row[i] = v;
    }
    perlin_state->cur = 1 - perlin_state->cur;
  }
  perlin_state->row = row;
  float *base_row =
      perlin_state->cur == 0 ? perlin_state->row_a : perlin_state->row_b;
  float *next_row =
      perlin_state->cur == 1 ? perlin_state->row_a : perlin_state->row_b;
  float nn = base_row[col];
  float xn = base_row[col + 1];
  float nx = next_row[col];
  float xx = next_row[col + 1];
  float y = curve3((i - row * cell_size) / cell_size);
  float Y = curve3(y);
  float _Y = 1 - Y;
  float x = curve3((j - col * cell_size) / cell_size);
  float X = curve3(x);
  float _X = 1 - X;
  float v = _X * _Y * nn + X * _Y * xn + _X * Y * nx + X * Y * xx;
  if (i == 0 && j == 0) {
    perlin_state->min_val = v;
    perlin_state->max_val = v;
  } else {
    if (v < perlin_state->min_val)
      perlin_state->min_val = v;
    if (v > perlin_state->max_val)
      perlin_state->max_val = v;
  }
  *reinterpret_cast<float *>(pixel.pixel) = v;
  //*static_cast<float *>(pixel) = nn[0] * 0.5 + 0.5;
}

bool PerlinValueGeneratorBuilder::SetIntParam(const std::string &param,
                                              int value) {
  if (param == "cell_size") {
    cell_size = value;
    return true;
  }
  return false;
}

void PerlinGradientGenerator::Generate(int i, int j, int rows, int cols,
                                       PixelSpecifier pixel) {}

void PerlinGradientGenerator::GenerateStateful(int i, int j, int rows, int cols,
                                               PixelSpecifier pixel,
                                               void *state) {
  PerlinState *perlin_state = static_cast<PerlinState *>(state);
  int row = static_cast<int>(floor(i / cell_size));
  int col = static_cast<int>(floor(j / cell_size));
  int row_size = static_cast<int>(ceil(cols / cell_size + 1));
  if (j == 0 && row != perlin_state->row) {
    float *row =
        perlin_state->cur == 0 ? perlin_state->row_a : perlin_state->row_b;
    for (int k = 0; k < row_size; k++) {
      float x = randGray() * 2 - 1;
      float y = randGray() * 2 - 1;
      float m = sqrt(x * x + y * y);
      row[2 * k] = x / m;
      row[2 * k + 1] = y / m;
    }
    perlin_state->cur = 1 - perlin_state->cur;
  }
  perlin_state->row = row;
  float *base_row =
      perlin_state->cur == 0 ? perlin_state->row_a : perlin_state->row_b;
  float *next_row =
      perlin_state->cur == 1 ? perlin_state->row_a : perlin_state->row_b;
  float *nn = base_row + 2 * col;
  float *xn = base_row + 2 * col + 2;
  float *nx = next_row + 2 * col;
  float *xx = next_row + 2 * col + 2;
  float y = (i - row * cell_size * 1.0) / cell_size;
  float _y = 1 - y;
  float Y = curve3(y);
  float _Y = 1 - Y;
  float x = (j - col * cell_size * 1.0) / cell_size;
  float _x = 1 - x;
  float X = curve3(x);
  float _X = 1 - X;
  float nnv = nn[0] * x + nn[1] * y;
  float xnv = -xn[0] * _x + xn[1] * y;
  float nxv = nx[0] * x - nx[1] * _y;
  float xxv = -xx[0] * _x - xx[1] * _y;
  float v = _X * _Y * nnv + X * _Y * xnv + _X * Y * nxv + X * Y * xxv;
  if (i == 0 && j == 0) {
    perlin_state->min_val = v;
    perlin_state->max_val = v;
  } else {
    if (v < perlin_state->min_val)
      perlin_state->min_val = v;
    if (v > perlin_state->max_val)
      perlin_state->max_val = v;
  }
  *reinterpret_cast<float *>(pixel.pixel) = v;
  //*static_cast<float *>(pixel) = nn[0] * 0.5 + 0.5;
}

bool PerlinGradientGeneratorBuilder::SetIntParam(const std::string &param,
                                                 int value) {
  if (param == "cell_size") {
    cell_size = value;
    return true;
  }
  return false;
}

namespace graphics {
namespace image {
namespace binary {
namespace GeneratorRegistrations {
GeneratorFactoryRegistration<PerlinValueGeneratorBuilder>
    _PerlinValueGeneratorBuilder("PerlinValueGenerator");
GeneratorFactoryRegistration<PerlinGradientGeneratorBuilder>
    _PerlinGradientGeneratorBuilder("PerlinGradientGenerator");
} // namespace GeneratorRegistrations
} // namespace binary
} // namespace image
} // namespace graphics