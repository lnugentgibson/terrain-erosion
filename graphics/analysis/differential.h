#ifndef GRAPHICS_ANALYSIS_DIFFERENTIAL_H
#define GRAPHICS_ANALYSIS_DIFFERENTIAL_H

namespace graphics {
namespace analysis {

struct Differential {
  float x_slope;
  float y_slope;
  float normal[3];
};

} // namespace analysis
} // namespace graphics

#endif // GRAPHICS_ANALYSIS_DIFFERENTIAL_H