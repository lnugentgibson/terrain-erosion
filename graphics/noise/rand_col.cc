#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "cxxopts/cxxopts.h"
#include "graphics/color/color.h"
#include "graphics/image/binary/binimg.h"
#include "graphics/noise/random.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a color image from binary to ppm");
  options.add_options()
    ("w,width", "Width of image", cxxopts::value<int>())
    ("h,height", "Height of image", cxxopts::value<int>())
    ("s,seed", "random seed", cxxopts::value<int>())
    ("c,count", "number of image", cxxopts::value<int>())
    ("o,output", "image filename prototype", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  int cols = result["w"].as<int>();
  int rows = result["h"].as<int>();
  int seed = result["s"].as<int>();
  if(seed < 0) {
    seed = time(NULL);
  }
  srand(seed);
  int count = result["c"].as<int>();
  for(int i = 0; i < count; i++) {
    std::stringstream fs;
    fs << result["o"].as<std::string>();
    fs << "_" << std::setfill('0') << std::setw(4) << rows;
    fs << "_" << std::setfill('0') << std::setw(4) << cols;
    fs << "_" << std::setfill('0') << std::setw(4) << seed;
    fs << "_" << std::setfill('0') << std::setw(3) << i;
    fs << ".bin";
    std::string filename = fs.str();
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    generateBin(rows, cols, 1, sizeof(Color), ofs, [](int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size) -> void {
      randCol(static_cast<Color *>(pixel));
    });
    ofs.close();
  }
  return 0;
}