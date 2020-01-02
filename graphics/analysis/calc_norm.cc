#include <cstdlib>
#include <cstring>
#include <iostream>

#include "cxxopts/cxxopts.h"
#include "graphics/analysis/differential.h"
#include "graphics/image/binary/binimg.h"

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a grayscale image from binary to ppm");
  options.add_options()
    ("i,input", "first binary image file path", cxxopts::value<std::string>())
    ("d,depth", "from min", cxxopts::value<float>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  float d = result["d"].as<float>();
  int smoothing = 16;
  std::ifstream ifs(result["i"].as<std::string>().c_str(), std::ios::in | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  auto builder = graphics::image::binary::TransformerFactory::get().Create("Differentiator");
  builder->SetFloatParam("d", d);
  auto transformer = (*builder)();
  graphics::image::binary::MapNeighborhood(sizeof(float), ifs, sizeof(graphics::analysis::Differential), 1, ofs, smoothing, transformer.get());
  ifs.close();
  ofs.close();
  return 0;
}