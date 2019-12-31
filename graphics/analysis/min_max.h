#ifndef MIN_MAX_H
#define MIN_MAX_H

#include "graphics/image/binary/binimg.h"

namespace graphics {
namespace analysis {

class MinMax : public image::binary::Accumulator {
  void Aggregate(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size, int n, void *aggregate) override;
};

} // namespace analysis
} // namespace graphics

#endif // MIN_MAX_H