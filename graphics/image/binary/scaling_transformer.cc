#include "graphics/image/binary/scaling_transformer.h"

namespace graphics {
namespace image {
namespace binary {

void ScalingTransformer::Transform(
  int i, int j, int rows, int cols,
  const void *pixel1, int dim1, size_t element_size1,
  void *pixel2, int dim2, size_t element_size2) {
  *static_cast<float *>(pixel2) = (static_cast<const float *>(pixel1)[0] - from_min) / from_range * to_range + to_min;
}

} // namespace binary
} // namespace image
} // namespace graphics