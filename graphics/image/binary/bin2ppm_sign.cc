#include <cstring>
#include <fstream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"

using graphics::image::InputSpecifier;
using graphics::image::binary::ColorizerFactory;
using graphics::image::binary::ToPPM;

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0],
                           "converts a grayscale image from binary to ppm");
  options.add_options()("i,input", "binary image file path",
                        cxxopts::value<std::string>())(
      "o,output", "ppm image file path", cxxopts::value<std::string>());
  auto result = options.parse(argc, argv);
  std::ifstream ifs(result["i"].as<std::string>().c_str(),
                    std::ios::out | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(),
                    std::ios::out | std::ios::binary);
  auto builder = ColorizerFactory::get().Create("SignedColorizer");
  auto colorizer = (*builder)();
  ToPPM(InputSpecifier(&ifs, sizeof(float)), ofs, colorizer.get());
  ifs.close();
  ofs.close();
  return 0;
}