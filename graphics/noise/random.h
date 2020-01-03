#ifndef RANDOM_H
#define RANDOM_H

#include "graphics/color/color.h"
#include "graphics/image/binary/binimg.h"

using graphics::image::binary::Generator;
using graphics::image::binary::GeneratorBuilder;
using graphics::image::binary::StatelessGenerator;

float randGray();

void randCol(Color *c);

void randVector(int dim, float *v);

void randDirection(int dim, float *v);

#endif // RANDOM_H