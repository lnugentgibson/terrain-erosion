#include "graphics/image/binary/util.h"

namespace graphics {
namespace image {
namespace binary {

void ScalingTransformer::Transform(
  int i, int j, int rows, int cols,
  const void *pixel1, int dim1, size_t element_size1,
  void *pixel2, int dim2, size_t element_size2) {
  *static_cast<float *>(pixel2) = (static_cast<const float *>(pixel1)[0] - from_min) / from_range * to_range + to_min;
}

bool ScalingTransformerBuilder::registered =
  TransformerFactory::Register(ScalingTransformerBuilder::GetFactoryName(),   
                                     ScalingTransformerBuilder::Create);

void MinMax::Aggregate(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size, int n, void *aggregate) {
  float v = static_cast<const float *>(pixel)[0];
  auto minmax = static_cast<std::pair<float, float>*>(aggregate);
  if(n == 0) {
    minmax->first = v;
    minmax->second = v;
  } else {
    if(v < minmax->first) {
      minmax->first = v;
    }
    if(v > minmax->second) {
      minmax->second = v;
    }
  }
}

void SumCombiner::Combine(
  int i, int j, int rows, int cols,
  const void *pixel1, int dim1, size_t element_size1,
  const void *pixel2, int dim2, size_t element_size2,
  void *pixel3, int dim3, size_t element_size3) {
  *static_cast<float *>(pixel3) = weight_a * static_cast<const float *>(pixel1)[0] + weight_b * static_cast<const float *>(pixel2)[0];
}

void GrayscaleColorizer::ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) {
  auto *v = static_cast<const float*>(pixel);
  rgb[0] = *v;
  rgb[1] = *v;
  rgb[2] = *v;
}

void SignedColorizer::ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) {
  auto *v = static_cast<const float*>(pixel);
  rgb[0] = *v * 0.5 + 0.5;
  rgb[1] = *v * 0.5 + 0.5;
  rgb[2] = *v * 0.5 + 0.5;
}

void ColorColorizer::ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) {
  auto *v = static_cast<const Color*>(pixel);
  rgb[0] = v->r();
  rgb[1] = v->g();
  rgb[2] = v->b();
}

void VectorColorizer::ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) {
  auto *v = static_cast<const float*>(pixel);
  rgb[0] = *v;
  rgb[1] = dim > 0 ? *v : 0.0;
  rgb[2] = dim > 1 ? *v : 0.0;
}

void DirectionColorizer::ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) {
  auto *v = static_cast<const float*>(pixel);
  rgb[0] = *v * 0.5 + 0.5;
  rgb[1] = dim > 0 ? *v * 0.5 + 0.5 : 0.5;
  rgb[2] = dim > 1 ? *v * 0.5 + 0.5 : 0.5;
}

} // namespace binary
} // namespace image
} // namespace graphics