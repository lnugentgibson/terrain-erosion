#include <cstring>
#include <fstream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"

using graphics::image::InputSpecifier;
using graphics::image::OutputSpecifier;
using graphics::image::binary::Map;
using graphics::image::binary::TransformerFactory;

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
  std::ifstream ifs(result["i"].as<std::string>().c_str(), std::ios::in | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(), std::ios::out | std::ios::binary);
  auto builder = TransformerFactory::get().Create("ScalingTransformer");
  builder->SetFloatParam("fn", fn);
  builder->SetFloatParam("fx", fx);
  builder->SetFloatParam("tn", tn);
  builder->SetFloatParam("tx", tx);
  auto transformer = (*builder)();
  Map(InputSpecifier(&ifs, sizeof(float)), OutputSpecifier(&ofs, sizeof(float)), transformer.get());
  ifs.close();
  ofs.close();
  return 0;
}