#ifndef NORMAL_H
#define NORMAL_H

#include "graphics/image/binary/binimg.h"

namespace graphics {
namespace analysis {
  
  using image::binary::Neighborhood;
  using image::binary::Transformer;
  using image::binary::TransformerBuilder;

struct Differential {
  float x_slope;
  float y_slope;
  float normal[3];
};

} // namespace analysis
} // namespace graphics

#endif // NORMAL_H