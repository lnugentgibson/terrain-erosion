#ifndef RANDOM_H
#define RANDOM_H

#include "graphics/color/color.h"
#include "graphics/image/binary/binimg.h"

float randGray();

void randCol(Color *c);

void randVector(int dim, float *v);

void randDirection(int dim, float *v);

#endif // RANDOM_H