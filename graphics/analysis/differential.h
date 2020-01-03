#ifndef NORMAL_H
#define NORMAL_H

namespace graphics {
namespace analysis {

struct Differential {
  float x_slope;
  float y_slope;
  float normal[3];
};

} // namespace analysis
} // namespace graphics

#endif // NORMAL_H