#include "graphics/analysis/min_max.h"

namespace graphics {
namespace analysis {

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

} // namespace analysis
} // namespace graphics