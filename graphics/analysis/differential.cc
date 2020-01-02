#include "graphics/analysis/differential.h"

#include <cmath>

#include "graphics/image/binary/binimg_registration.h"

namespace graphics {
namespace analysis {

class Differentiator : public Transformer {
  float depth;
 public:
  Differentiator(float d) : depth(d) {}
  virtual void Transform(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2) override {
      return;
    }
  
  virtual void TransformStateful(
    int i, int j, int rows, int cols,
    const void *pixel1, int dim1, size_t element_size1,
    void *pixel2, int dim2, size_t element_size2,
    void *state) override {
      return;
    }
  
  virtual void TransformNeighborhood(
    int i, int j, int rows, int cols,
    Neighborhood&& neighborhood,
    void *pixel2, int dim2, size_t element_size2) override;
};
  
class DifferentiatorBuilder : public TransformerBuilder {
  float d;
 public:
  std::unique_ptr<Transformer> operator ()() override {
    return std::unique_ptr<Transformer>(new Differentiator(d));
  }
  bool SetIntParam(const std::string& param, int value) override { return false; }
  bool SetFloatParam(const std::string& param, float value) override;
};

void Differentiator::TransformNeighborhood(
  int i, int j, int rows, int cols,
  image::binary::Neighborhood&& neighborhood,
  void *pixel2, int dim2, size_t element_size2) {
  auto r = neighborhood.range();
  Differential *diff = static_cast<Differential*>(pixel2);
  auto x_neg = neighborhood.get(0, r[2]);
  auto x_pos = neighborhood.get(0, r[3] - 1);
  auto y_neg = neighborhood.get(r[0], 0);
  auto y_pos = neighborhood.get(r[1] - 1, 0);
  diff->x_slope = depth * (*reinterpret_cast<float *>(x_pos) - *reinterpret_cast<float *>(x_neg)) * cols / (r[3] - r[2] - 1);
  diff->y_slope = depth * (*reinterpret_cast<float *>(y_pos) - *reinterpret_cast<float *>(y_neg)) * rows / (r[1] - r[0] - 1);
  delete[] x_neg;
  delete[] x_pos;
  delete[] y_neg;
  delete[] y_pos;
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

} // namespace analysis

namespace image {
namespace binary {
namespace TransformerRegistrations {
	TransformerFactoryRegistration<analysis::DifferentiatorBuilder> _DifferentiatorBuilder("Differentiator");
}
} // namespace binary
} // namespace image

} // namespace graphics