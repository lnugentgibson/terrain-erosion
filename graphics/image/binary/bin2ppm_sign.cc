#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"

class SignedColorizer : public graphics::image::binary::Colorizer {
 public:
  void ToRGB(const void *pixel, int dim, size_t element_size, float *rgb) override {
    auto *v = static_cast<const float*>(pixel);
    rgb[0] = *v * 0.5 + 0.5;
    rgb[1] = *v * 0.5 + 0.5;
    rgb[2] = *v * 0.5 + 0.5;
  }
};

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a grayscale image from binary to ppm");
  options.add_options()
    ("i,input", "binary image file path", cxxopts::value<std::string>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  std::ifstream ifs(result["i"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  SignedColorizer colorizer;
  graphics::image::binary::ToPPM(sizeof(float), ifs, ofs, &colorizer);
  ifs.close();
  ofs.close();
  return 0;
}