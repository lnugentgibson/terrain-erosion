#ifndef GRAPHICS_NOISE_NOISE_H
#define GRAPHICS_NOISE_NOISE_H

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

#endif // GRAPHICS_NOISE_NOISE_H