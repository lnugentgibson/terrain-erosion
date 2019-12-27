#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts.hpp"
#include "binimg.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a grayscale image from binary to ppm");
  options.add_options()
    ("i,input", "first binary image file path", cxxopts::value<std::string>())
    ("f,from-min", "from min", cxxopts::value<float>())
    ("g,from-max", "from max", cxxopts::value<float>())
    ("t,to-min", "to min", cxxopts::value<float>())
    ("u,to-max", "to max", cxxopts::value<float>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  float fn = result["f"].as<float>();
  float fx = result["g"].as<float>();
  float tn = result["t"].as<float>();
  float tx = result["u"].as<float>();
  float fr = fx - fn;
  float tr = tx - tn;
  transformBin<float, float>(result["i"].as<std::string>().c_str(), result["o"].as<std::string>().c_str(), 1, [fn, fr, tr, tn](const float *pixel1, int dim1, float *pixel2, int dim2) -> void {
    pixel2[0] = (pixel1[0] - fn) / fr * tr + tn;
  });
  return 0;
}