#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a grayscale image from binary to ppm");
  options.add_options()
    ("i,input", "first binary image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  std::ifstream ifs(result["i"].as<std::string>().c_str(), std::ios::in | std::ios::binary);
  auto minmax = reduceBin<std::pair<float, float>>(sizeof(float), ifs, [](int i, int j, int rows, int cols, void *pixel, int dim, size_t element_size, int n, std::pair<float, float> *aggregate) -> void {
    float v = static_cast<float *>(pixel)[0];
    if(n == 0) {
      aggregate->first = v;
      aggregate->second = v;
    } else {
      if(v < aggregate->first) {
        aggregate->first = v;
      }
      if(v > aggregate->second) {
        aggregate->second = v;
      }
    }
  });
  ifs.close();
  std::cout << "--from-min=" << minmax.first << " --from-max=" << minmax.second << std::endl;
  return 0;
}