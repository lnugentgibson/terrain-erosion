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

#endif // NOISE_H