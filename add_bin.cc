#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts.hpp"
#include "binimg.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a grayscale image from binary to ppm");
  options.add_options()
    ("a,input-a", "first binary image file path", cxxopts::value<std::string>())
    ("b,input-b", "second binary image file path", cxxopts::value<std::string>())
    ("m,mult-a", "first binary image file path", cxxopts::value<float>())
    ("n,mult-b", "second binary image file path", cxxopts::value<float>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  combineBin<float, float, float>(result["a"].as<std::string>().c_str(), result["b"].as<std::string>().c_str(), result["o"].as<std::string>().c_str(), 1, [&result](const float *a, const float *b, float *c) -> void {
    c[0] = result["m"].as<float>() * a[0] + result["n"].as<float>() * b[0];
  });
  return 0;
}