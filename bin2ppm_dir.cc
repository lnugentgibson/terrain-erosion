#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts.hpp"
#include "binimg.h"
#include "random.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a normal image from binary to ppm");
  options.add_options()
    ("i,input", "binary image file path", cxxopts::value<std::string>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  std::ifstream ifs(result["i"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  ppmBin(sizeof(float), ifs, ofs, [](void *pixel, int dim, size_t element_size, float *rgb) -> void {
    rgb[0] = static_cast<float *>(pixel)[0];
    rgb[1] = dim > 0 ? static_cast<float *>(pixel)[1] : 0.0;
    rgb[2] = dim > 1 ? static_cast<float *>(pixel)[2] : 0.0;
  });
  return 0;
}