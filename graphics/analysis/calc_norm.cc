#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts.hpp"
#include "binimg.h"
#include "normal.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a grayscale image from binary to ppm");
  options.add_options()
    ("i,input", "first binary image file path", cxxopts::value<std::string>())
    ("d,depth", "from min", cxxopts::value<float>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  float d = result["d"].as<float>();
  int smoothing = 16;
  //*
  transformBinNeighbors<float, Differential>(result["i"].as<std::string>().c_str(), result["o"].as<std::string>().c_str(), 1, 16, [d](int rows, int cols, const Neighborhood<float> *n, int dim1, Differential *pixel2, int dim2) -> void {
    auto r = n->range();
    pixel2->x_slope = (*n->get(0, r[3] - 1) - *n->get(0, r[2])) * cols / (r[3] - r[2] - 1);
    pixel2->y_slope = (*n->get(r[1] - 1, 0) - *n->get(r[0], 0)) * rows / (r[1] - r[0] - 1);
    float g = sqrt(pixel2->x_slope * pixel2->x_slope + pixel2->y_slope * pixel2->y_slope);
    float f = 2.0;
    float _g = sqrt(g * g / (f * f) + 1);
    pixel2->normal[0] = -pixel2->x_slope / (f * _g);
    pixel2->normal[1] = -pixel2->y_slope / (f * _g);
    pixel2->normal[2] = 1.0 / _g;
  });
  //*/
  return 0;
}