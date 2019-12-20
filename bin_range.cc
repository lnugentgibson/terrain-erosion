#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts.hpp"
#include "binimg.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a grayscale image from binary to ppm");
  options.add_options()
    ("i,input", "first binary image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  auto minmax = summarizeBin<float, std::pair<float, float>>(result["i"].as<std::string>().c_str(), [](const float *pixel, int dim, int index, std::pair<float, float> *summary) -> void {
    if(index == 0) {
      summary->first = pixel[0];
      summary->second = pixel[0];
    } else {
      if(pixel[0] < summary->first) {
        summary->first = pixel[0];
      }
      if(pixel[0] > summary->second) {
        summary->second = pixel[0];
      }
    }
  });
  std::cout << "--from-min=" << minmax.first << " --from-max=" << minmax.second << std::endl;
  return 0;
}