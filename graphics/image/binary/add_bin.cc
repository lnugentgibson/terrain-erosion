#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"
#include "graphics/image/binary/util.h"

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
  std::ifstream ifs1(result["a"].as<std::string>().c_str(), std::ios::in | std::ios::binary);
  std::ifstream ifs2(result["b"].as<std::string>().c_str(), std::ios::in | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  graphics::image::binary::SumCombiner combiner(result["m"].as<float>(), result["n"].as<float>());
  graphics::image::binary::Combine(sizeof(float), ifs1, sizeof(float), ifs2, sizeof(float), 1, ofs, &combiner);
  ifs1.close();
  ifs2.close();
  ofs.close();
  return 0;
}