#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

using graphics::image::binary::Colorizer;
using graphics::image::binary::ColorizerFactory;
using graphics::image::binary::InputSpecifier;
using graphics::image::binary::PixelSpecifier;
using graphics::image::binary::StatelessFunctor;
using graphics::image::binary::ToPPM;

class ToPNG : public StatelessFunctor {
  Colorizer *colorizer;
  uint8_t *pixels;
 public:
  ToPNG(Colorizer *c, uint8_t *ps) : colorizer(c), pixels(ps) {}
  void Do(int i, int j, int rows, int cols, const PixelSpecifier pixel) override {
    float *rgb = new float[3];
    colorizer->ToRGB(pixel, rgb);
    int irgb[3];
    for(int c = 0; c < 3; c++) {
      irgb[c] = (int) (256 * rgb[c]);
      if(irgb[c] > 255) irgb[c] = 255;
      if(irgb[c] < 0) irgb[c] = 0;
      pixels[(i * cols + j) * 3 + c] = (uint8_t) irgb[c];
    }
  }
};

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a grayscale image from binary to ppm");
  options.add_options()
    ("i,input", "binary image file path", cxxopts::value<std::string>())
    ("w,width", "Width of image", cxxopts::value<int>())
    ("h,height", "Height of image", cxxopts::value<int>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  int cols = result["w"].as<int>();
  int rows = result["h"].as<int>();
  std::ifstream ifs(result["i"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  auto builder = ColorizerFactory::get().Create("SignedColorizer");
  auto colorizer = (*builder)();
  uint8_t *pixels = new uint8_t[cols * rows * 3];
  ForEach(InputSpecifier(&ifs, sizeof(float)), new ToPNG(colorizer.get(), pixels));
  ifs.close();
  stbi_write_png(result["o"].as<std::string>().c_str(), cols, rows, 3, pixels, cols * 3);
  delete[] pixels;
  return 0;
}