#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts.hpp"
#include "grid.h"
#include "random.h"
#include "noise.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a color image from binary to ppm");
  options.add_options()
    ("w,width", "Width of image", cxxopts::value<int>())
    ("h,height", "Height of image", cxxopts::value<int>())
    ("c,cell-size", "Height of image", cxxopts::value<int>())
    ("t,type", "format of output image", cxxopts::value<int>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  bool bin = !!result["t"].as<int>();
  std::string filename = result["o"].as<std::string>();
  filename = filename + (bin ? ".bin" : ".ppm");
  int cell_size = result["c"].as<int>();
  Grid<float, GrayscaleComponent> r_grid(result["h"].as<int>() / cell_size + 1, result["w"].as<int>() / cell_size + 1, 2);
  r_grid.allocate();
  Grid<float, GrayscaleComponent> grid(result["h"].as<int>(), result["w"].as<int>(), 1);
  grid.allocate();
  randGrid(r_grid);
  perlinGradientGrid(grid, r_grid, cell_size);
  if(bin) {
    grid.saveBin(filename.c_str());
  } else {
    grid.savePpm(filename.c_str());
  }
  return 0;
}