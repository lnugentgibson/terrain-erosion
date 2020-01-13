#ifndef GRAPHICS_NOISE_RANDOM_H
#define GRAPHICS_NOISE_RANDOM_H

#include "graphics/color/color.h"

float randGray();

void randCol(Color *c);

void randVector(int dim, float *v);

void randDirection(int dim, float *v);

#endif // GRAPHICS_NOISE_RANDOM_H