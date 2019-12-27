#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

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
    ("c,count", "number of image", cxxopts::value<int>())
    ("t,type", "format of output image", cxxopts::value<int>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  bool bin = !!result["t"].as<int>();
  int width = result["w"].as<int>();
  int height = result["h"].as<int>();
  int dim = result["d"].as<int>();
  Grid<float, ArrayComponent> grid(height, width, dim);
  grid.allocate();
  int seed = result["s"].as<int>();
  if(seed < 0) {
    seed = time(NULL);
  }
  srand(seed);
  int count = result["c"].as<int>();
  for(int i = 0; i < count; i++) {
    std::stringstream fs;
    fs << result["o"].as<std::string>();
    fs << "_" << std::setfill('0') << std::setw(4) << height;
    fs << "_" << std::setfill('0') << std::setw(4) << width;
    fs << "_" << dim;
    fs << "_" << std::setfill('0') << std::setw(4) << seed;
    fs << "_" << std::setfill('0') << std::setw(3) << i;
    fs << (bin ? ".bin" : ".ppm");
    std::string filename = fs.str();
    randGridVector(grid);
    if(bin) {
      grid.saveBin(filename.c_str());
    } else {
      grid.savePpm(filename.c_str());
    }
  }
  return 0;
}