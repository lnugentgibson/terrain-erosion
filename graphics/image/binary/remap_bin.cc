#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"

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
  std::ifstream ifs(result["i"].as<std::string>().c_str(), std::ios::in | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  mapBin(sizeof(float), ifs, sizeof(float), 1, ofs, [fn, fr, tr, tn](int i, int j, int rows, int cols, void *pixel1, int dim1, size_t element_size1, void *pixel2, int dim2, size_t element_size2) -> void {
    *static_cast<float *>(pixel2) = (static_cast<float *>(pixel1)[0] - fn) / fr * tr + tn;
  });
  ifs.close();
  ofs.close();
  return 0;
}