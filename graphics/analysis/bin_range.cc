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
  std::pair<float, float> minmax;
  auto builder = graphics::image::binary::AccumulatorFactory::get().Create("MinMaxAccumulator");
  auto reducer = (*builder)();
  graphics::image::binary::Reduce(sizeof(float), ifs, reducer.get(), &minmax);
  ifs.close();
  std::cout << "--from-min=" << minmax.first << " --from-max=" << minmax.second << std::endl;
  return 0;
}