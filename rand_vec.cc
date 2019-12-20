#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts.hpp"
#include "grid.h"
#include "random.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a color image from binary to ppm");
  options.add_options()
    ("w,width", "Width of image", cxxopts::value<int>())
    ("h,height", "Height of image", cxxopts::value<int>())
    ("d,dim", "Dimensions of image", cxxopts::value<int>())
    ("s,seed", "random seed", cxxopts::value<int>())
    ("t,type", "format of output image", cxxopts::value<int>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  bool bin = !!result["t"].as<int>();
  std::string filename = result["o"].as<std::string>();
  // filename = filename + (bin ? ".bin" : ".ppm");
  Grid<float, ArrayComponent> grid(result["h"].as<int>(), result["w"].as<int>(), result["d"].as<int>());
  grid.allocate();
  int seed = result["s"].as<int>();
  if(seed < 0) {
    seed = time(NULL);
  }
  srand(seed);
  randGridVector(grid);
  if(bin) {
    grid.saveBin(filename.c_str());
  } else {
    grid.savePpm(filename.c_str());
  }
  return 0;
}