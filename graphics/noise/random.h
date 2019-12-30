#ifndef RANDOM_H
#define RANDOM_H

#include "graphics/color/color.h"
#include "graphics/image/binary/binimg.h"

float randGray();

class RandomGrayscaleGenerator : public graphics::image::binary::StatelessGenerator {
 public:
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) override;
};

void randCol(Color *c);

class RandomColorGenerator : public graphics::image::binary::StatelessGenerator {
 public:
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) override;
};

void randVector(int dim, float *v);

class RandomVectorGenerator : public graphics::image::binary::StatelessGenerator {
 public:
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) override;
};

void randDirection(int dim, float *v);

class RandomDirectionGenerator : public graphics::image::binary::StatelessGenerator {
 public:
  void Generate(int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) override;
};

#endif // RANDOM_H