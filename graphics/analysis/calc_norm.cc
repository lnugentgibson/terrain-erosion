#include <cstring>
#include <fstream>
#include <sstream>

#include "cxxopts/cxxopts.h"
#include "graphics/analysis/differential.h"
#include "graphics/image/binary/binimg.h"

using graphics::analysis::Differential;
using graphics::image::InputSpecifier;
using graphics::image::OutputSpecifier;
using graphics::image::binary::MapNeighborhood;
using graphics::image::binary::TransformerFactory;

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0],
                           "converts a grayscale image from binary to ppm");
  options.add_options()("i,input", "first binary image file path",
                        cxxopts::value<std::string>())(
      "d,depth", "from min", cxxopts::value<float>())("s,smoothing", "from min",
                                                      cxxopts::value<int>())(
      "o,output", "ppm image file path", cxxopts::value<std::string>());
  auto result = options.parse(argc, argv);

  float d = result["d"].as<float>();
  int smoothing = result["s"].as<int>();
  std::ifstream ifs(result["i"].as<std::string>().c_str(),
                    std::ios::in | std::ios::binary);
  std::stringstream fsd;
  fsd << result["o"].as<std::string>() << "_diff.bin";
  std::string filenamed = fsd.str();
  std::ofstream ofsd(filenamed.c_str(), std::ios::out | std::ios::binary);
  auto builder = TransformerFactory::get().Create("Differentiator");
  builder->SetFloatParam("d", d);
  auto transformer = (*builder)();
  MapNeighborhood(InputSpecifier(&ifs, sizeof(float)),
                  OutputSpecifier(&ofsd, sizeof(Differential)), smoothing,
                  transformer.get());
  ifs.close();
  ofsd.close();

  std::ifstream ifsd(filenamed.c_str(), std::ios::in | std::ios::binary);

  std::stringstream fsx;
  fsx << result["o"].as<std::string>() << "_dx.bin";
  std::string filenamex = fsx.str();
  std::ofstream ofsx(filenamex.c_str(), std::ios::out | std::ios::binary);
  builder = TransformerFactory::get().Create("DifferentialDXExtractor");
  transformer = (*builder)();
  Map(InputSpecifier(&ifsd, sizeof(Differential)),
      OutputSpecifier(&ofsx, sizeof(float)), transformer.get());
  ofsx.close();

  // ifsd.clear();
  ifsd.seekg(0, std::ios::beg);
  std::stringstream fsy;
  fsy << result["o"].as<std::string>() << "_dy.bin";
  std::string filenamey = fsy.str();
  std::ofstream ofsy(filenamey.c_str(), std::ios::out | std::ios::binary);
  builder = TransformerFactory::get().Create("DifferentialDYExtractor");
  transformer = (*builder)();
  Map(InputSpecifier(&ifsd, sizeof(Differential)),
      OutputSpecifier(&ofsy, sizeof(float)), transformer.get());
  ofsy.close();

  // ifsd.clear();
  ifsd.seekg(0, std::ios::beg);
  std::stringstream fsn;
  fsn << result["o"].as<std::string>() << "_norm.bin";
  std::string filenamen = fsn.str();
  std::ofstream ofsn(filenamen.c_str(), std::ios::out | std::ios::binary);
  builder = TransformerFactory::get().Create("DifferentialNormalExtractor");
  transformer = (*builder)();
  Map(InputSpecifier(&ifsd, sizeof(Differential)),
      OutputSpecifier(&ofsn, sizeof(float), 3), transformer.get());
  ofsn.close();

  ifsd.close();
  return 0;
}