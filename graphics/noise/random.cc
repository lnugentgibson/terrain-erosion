#include "random.h"

#include <cmath>

#include "graphics/image/binary/binimg_registration.h"

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

float randGray() {
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void RandomGrayscaleGenerator::Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) {
  *static_cast<float *>(pixel) = randGray();
}

void randCol(Color *c) {
  c->r(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
  c->g(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
  c->b(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

void RandomColorGenerator::Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) {
  randCol(static_cast<Color *>(pixel));
}

void randVector(int dim, float *v) {
  for(int k = 0; k < dim; k++) {
    v[k] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5;
  }
}

void RandomVectorGenerator::Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) {
  randVector(dim, static_cast<float *>(pixel));
}

void randDirection(int dim, float *v) {
  float l = 0;
  for(int k = 0; k < dim; k++) {
    v[k] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5;
    l += v[k] * v[k];
  }
  l = sqrt(l);
  for(int k = 0; k < dim; k++) {
    v[k] = v[k] / l;
  }
}

void RandomDirectionGenerator::Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) {
  randDirection(dim, static_cast<float *>(pixel));
}

namespace graphics {
namespace image {
namespace binary {
namespace GeneratorRegistrations {
	GeneratorFactoryRegistration<RandomGrayscaleGeneratorBuilder> _RandomGrayscaleGeneratorBuilder("RandomGrayscaleGenerator");
	GeneratorFactoryRegistration<RandomColorGeneratorBuilder> _RandomColorGeneratorBuilder("RandomColorGenerator");
	GeneratorFactoryRegistration<RandomVectorGeneratorBuilder> _RandomVectorGeneratorBuilder("RandomVectorGenerator");
	GeneratorFactoryRegistration<RandomDirectionGeneratorBuilder> _RandomDirectionGeneratorBuilder("RandomDirectionGenerator");
}
} // namespace binary
} // namespace image
} // namespace graphics