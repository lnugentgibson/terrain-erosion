#ifndef NORMAL_H
#define NORMAL_H

#include "graphics/image/binary/binimg.h"

namespace graphics {
namespace analysis {

struct Differential {
  float x_slope;
  float y_slope;
  float normal[3];
};

class Differentiator : public image::binary::Transformer {
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
    image::binary::Neighborhood&& neighborhood,
    void *pixel2, int dim2, size_t element_size2) override;
};

} // namespace analysis
} // namespace graphics

#endif // NORMAL_H