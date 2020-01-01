#ifndef UTIL_H
#define UTIL_H

#include <memory>

#include "graphics/color/color.h"
#include "graphics/image/binary/binimg.h"
#include "graphics/image/binary/binimg_registration.h"

namespace graphics {
namespace image {
namespace binary {

class ScalingTransformer : public SimpleTransformer {
  float from_min, from_max, from_range, to_min, to_max, to_range;
 public:
  ScalingTransformer(float fn, float fx, float tn, float tx) : from_min(fn), from_max(fx), from_range(from_max - from_min), to_min(tn), to_max(tx), to_range(to_max - to_min) {}
  void Transform(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2) override;
};
  
class ScalingTransformerBuilder : public TransformerBuilder {
  float fn, fx, tn, tx;
 public:
  std::unique_ptr<Transformer> operator ()() override {
    return std::unique_ptr<Transformer>(new ScalingTransformer(fn, fx, tn, tx));
  }
  bool SetFloatParam(const std::string& param, float value) override;
};

class MinMax : public image::binary::Accumulator {
  void Aggregate(int i, int j, int rows, int cols, const void *pixel, int dim, size_t element_size, int n, void *aggregate) override;
};

class SumCombiner : public graphics::image::binary::StatelessCombiner {
  float weight_a, weight_b;
 public:
  SumCombiner(float wa, float wb) : weight_a(wa), weight_b(wb) {}
  void Combine(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    const void *pixel2, int dim2, size_t element_size2,
    void *pixel3, int dim3, size_t element_size3) override;
};

class GrayscaleColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) override;
};

class SignedColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) override;
};

class ColorColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) override;
};

class VectorColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) override;
};

class DirectionColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) override;
};

} // namespace binary
} // namespace image
} // namespace graphics

#endif // UTIL_H