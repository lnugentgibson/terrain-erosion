#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts.hpp"
#include "grid.h"
#include "random.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a normal image from binary to ppm");
  options.add_options()
    ("i,input", "binary image file path", cxxopts::value<std::string>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  auto grid = readBin<float, NormalComponent>(result["i"].as<std::string>().c_str());
  grid.savePpm(result["o"].as<std::string>().c_str());
  return 0;
}