#include "random.h"

#include <cmath>

float randGray() {
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void randVector(int dim, float *v) {
  for(int k = 0; k < dim; k++) {
    v[k] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5;
  }
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

void randCol(Color *c) {
  c->r(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
  c->g(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
  c->b(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}