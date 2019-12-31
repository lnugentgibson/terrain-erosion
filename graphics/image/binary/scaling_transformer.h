#ifndef SCALING_TRANSFORMER
#define SCALING_TRANSFORMER

#include "graphics/image/binary/binimg.h"

namespace graphics {
namespace image {
namespace binary {

class ScalingTransformer : public SimpleTransformer {
  float from_min, from_max, from_range, to_min, to_max, to_range;
 public:
  ScalingTransformer(float fn, float fx, float tn, float tx) : from_min(fn), from_max(fx), from_range(from_max - from_min), to_min(tn), to_max(tx), to_range(to_max - to_min) {}
  virtual void Transform(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2) override;
};

} // namespace binary
} // namespace image
} // namespace graphics

#endif // SCALING_TRANSFORMER