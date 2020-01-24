#include "graphics/analysis/differential.h"

#include <cmath>

#include "graphics/image/binary/binimg.h"
#include "graphics/image/binary/binimg_registration.h"

namespace graphics {
namespace analysis {
  
using image::binary::Neighborhood;
using image::binary::PixelSpecifier;
using image::binary::SimpleTransformer;
using image::binary::Transformer;
using image::binary::TransformerBuilder;

class Differentiator : public Transformer {
  float depth;
 public:
  Differentiator(float d) : depth(d) {}
  virtual void Transform(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel) override {
      return;
    }
  
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel,
    void *state) override {
      return;
    }
  
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    const Neighborhood& neighborhood,
    PixelSpecifier out_pixel) override;
};
  
class DifferentiatorBuilder : public TransformerBuilder {
  float d;
 public:
  std::unique_ptr<Transformer> operator ()() override {
    return std::unique_ptr<Transformer>(new Differentiator(d));
  }
  bool SetFloatParam(const std::string& param, float value) override;
};

void Differentiator::TransformNeighborhood(
  int i, int j, int rows, int cols,
  const Neighborhood& neighborhood,
  PixelSpecifier out_pixel) {
  auto r = neighborhood.range();
  Differential *diff = reinterpret_cast<Differential*>(out_pixel.pixel);
  float x_neg = *reinterpret_cast<float *>(neighborhood.get(0, r[2]).pixel);
  auto x_pos = *reinterpret_cast<float *>(neighborhood.get(0, r[3] - 1).pixel);
  auto y_neg = *reinterpret_cast<float *>(neighborhood.get(r[0], 0).pixel);
  auto y_pos = *reinterpret_cast<float *>(neighborhood.get(r[1] - 1, 0).pixel);
  diff->x_slope = depth * (x_pos - x_neg) * cols / (r[3] - r[2] - 1);
  diff->y_slope = depth * (y_pos - y_neg) * rows / (r[1] - r[0] - 1);
  float g = sqrt(diff->x_slope * diff->x_slope + diff->y_slope * diff->y_slope);
  float f = 2.0;
  float _g = sqrt(g * g / (f * f) + 1);
  diff->normal[0] = -diff->x_slope / (f * _g);
  diff->normal[1] = -diff->y_slope / (f * _g);
  diff->normal[2] = 1.0 / _g;
}

bool DifferentiatorBuilder::SetFloatParam(const std::string& param, float value) {
  if(param == "d") {
    d = value;
    return true;
  }
  return false;
}

class DifferentialDXExtractor : public SimpleTransformer {
 public:
  virtual void Transform(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel) override;
};
  
class DifferentialDXExtractorBuilder : public TransformerBuilder {
 public:
  std::unique_ptr<Transformer> operator ()() override {
    return std::unique_ptr<Transformer>(new DifferentialDXExtractor());
  }
};

void DifferentialDXExtractor::Transform(
  int i, int j, int rows, int cols,
  const PixelSpecifier in_pixel,
  PixelSpecifier out_pixel) {
  const Differential *diff = reinterpret_cast<const Differential*>(in_pixel.pixel);
  *reinterpret_cast<float*>(out_pixel.pixel) = diff->x_slope;
}

class DifferentialDYExtractor : public SimpleTransformer {
 public:
  virtual void Transform(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel) override;
};
  
class DifferentialDYExtractorBuilder : public TransformerBuilder {
 public:
  std::unique_ptr<Transformer> operator ()() override {
    return std::unique_ptr<Transformer>(new DifferentialDYExtractor());
  }
};

void DifferentialDYExtractor::Transform(
  int i, int j, int rows, int cols,
  const PixelSpecifier in_pixel,
  PixelSpecifier out_pixel) {
  const Differential *diff = reinterpret_cast<const Differential*>(in_pixel.pixel);
  *reinterpret_cast<float*>(out_pixel.pixel) = diff->y_slope;
}

class DifferentialNormalExtractor : public SimpleTransformer {
 public:
  virtual void Transform(
    int i, int j, int rows, int cols,
    const PixelSpecifier in_pixel,
    PixelSpecifier out_pixel) override;
};
  
class DifferentialNormalExtractorBuilder : public TransformerBuilder {
 public:
  std::unique_ptr<Transformer> operator ()() override {
    return std::unique_ptr<Transformer>(new DifferentialNormalExtractor());
  }
};

void DifferentialNormalExtractor::Transform(
  int i, int j, int rows, int cols,
  const PixelSpecifier in_pixel,
  PixelSpecifier out_pixel) {
  const Differential *diff = reinterpret_cast<const Differential*>(in_pixel.pixel);
  for(int i = 0; i < 3; i++)
    reinterpret_cast<float*>(out_pixel.pixel)[i] = diff->normal[i];
}

} // namespace analysis

namespace image {
namespace binary {
namespace TransformerRegistrations {
	TransformerFactoryRegistration<analysis::DifferentiatorBuilder> _DifferentiatorBuilder("Differentiator");
	TransformerFactoryRegistration<analysis::DifferentialDXExtractorBuilder> _DifferentialDXExtractorBuilder("DifferentialDXExtractor");
	TransformerFactoryRegistration<analysis::DifferentialDYExtractorBuilder> _DifferentialDYExtractorBuilder("DifferentialDYExtractor");
	TransformerFactoryRegistration<analysis::DifferentialNormalExtractorBuilder> _DifferentialNormalExtractorBuilder("DifferentialNormalExtractor");
}
} // namespace binary
} // namespace image

} // namespace graphics