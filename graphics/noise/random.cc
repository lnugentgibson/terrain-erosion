#include "random.h"

#include <cmath>

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