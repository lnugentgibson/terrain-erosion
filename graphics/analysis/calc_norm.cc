#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"
#include "graphics/analysis/normal.h"

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
  std::ifstream ifs(result["i"].as<std::string>().c_str(), std::ios::in | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  mapNeighborhoodBin(sizeof(float), ifs, sizeof(Differential), 1, ofs, smoothing, [d](int i, int j, int rows, int cols, Neighborhood neighborhood, void *pixel2, int dim2, size_t element_size2) -> void {
    auto r = neighborhood.range();
    Differential *diff = static_cast<Differential*>(pixel2);
    auto x_neg = neighborhood.get(0, r[2]);
    auto x_pos = neighborhood.get(0, r[3] - 1);
    auto y_neg = neighborhood.get(r[0], 0);
    auto y_pos = neighborhood.get(r[1] - 1, 0);
    diff->x_slope = d * (*reinterpret_cast<float *>(x_pos) - *reinterpret_cast<float *>(x_neg)) * cols / (r[3] - r[2] - 1);
    diff->y_slope = d * (*reinterpret_cast<float *>(y_pos) - *reinterpret_cast<float *>(y_neg)) * rows / (r[1] - r[0] - 1);
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
  });
  ifs.close();
  ofs.close();
  return 0;
}