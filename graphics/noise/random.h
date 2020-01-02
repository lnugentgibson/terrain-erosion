#ifndef RANDOM_H
#define RANDOM_H

#include "graphics/color/color.h"
#include "graphics/image/binary/binimg.h"

using graphics::image::binary::Generator;
using graphics::image::binary::GeneratorBuilder;
using graphics::image::binary::StatelessGenerator;

float randGray();

class RandomGrayscaleGenerator : public StatelessGenerator {
 public:
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) override;
};
  
class RandomGrayscaleGeneratorBuilder : public GeneratorBuilder {
 public:
  std::unique_ptr<Generator> operator ()() override {
    return std::unique_ptr<Generator>(new RandomGrayscaleGenerator());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

void randCol(Color *c);

class RandomColorGenerator : public StatelessGenerator {
 public:
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) override;
};
  
class RandomColorGeneratorBuilder : public GeneratorBuilder {
 public:
  std::unique_ptr<Generator> operator ()() override {
    return std::unique_ptr<Generator>(new RandomColorGenerator());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

void randVector(int dim, float *v);

class RandomVectorGenerator : public StatelessGenerator {
 public:
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) override;
};
  
class RandomVectorGeneratorBuilder : public GeneratorBuilder {
 public:
  std::unique_ptr<Generator> operator ()() override {
    return std::unique_ptr<Generator>(new RandomVectorGenerator());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

void randDirection(int dim, float *v);

class RandomDirectionGenerator : public StatelessGenerator {
 public:
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) override;
};
  
class RandomDirectionGeneratorBuilder : public GeneratorBuilder {
 public:
  std::unique_ptr<Generator> operator ()() override {
    return std::unique_ptr<Generator>(new RandomDirectionGenerator());
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override { return false; }
};

#endif // RANDOM_H