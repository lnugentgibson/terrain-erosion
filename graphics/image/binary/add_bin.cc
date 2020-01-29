#include <cstring>
#include <fstream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"

using graphics::image::InputSpecifier;
using graphics::image::OutputSpecifier;
using graphics::image::binary::Combine;
using graphics::image::binary::CombinerFactory;

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0],
                           "converts a grayscale image from binary to ppm");
  options.add_options()("a,input-a", "first binary image file path",
                        cxxopts::value<std::string>())(
      "b,input-b", "second binary image file path",
      cxxopts::value<std::string>())("m,mult-a", "first binary image file path",
                                     cxxopts::value<float>())(
      "n,mult-b", "second binary image file path", cxxopts::value<float>())(
      "o,output", "ppm image file path", cxxopts::value<std::string>());
  auto result = options.parse(argc, argv);
  std::ifstream ifs1(result["a"].as<std::string>().c_str(),
                     std::ios::in | std::ios::binary);
  std::ifstream ifs2(result["b"].as<std::string>().c_str(),
                     std::ios::in | std::ios::binary);
  std::ofstream ofs(result["o"].as<std::string>().c_str(),
                    std::ios::out | std::ios::binary);
  auto builder = CombinerFactory::get().Create("SumCombiner");
  builder->SetFloatParam("wa", result["m"].as<float>());
  builder->SetFloatParam("wb", result["n"].as<float>());
  auto combiner = (*builder)();
  Combine(InputSpecifier(&ifs1, sizeof(float)),
          InputSpecifier(&ifs2, sizeof(float)),
          OutputSpecifier(&ofs, sizeof(float)), combiner.get());
  ifs1.close();
  ifs2.close();
  ofs.close();
  return 0;
}