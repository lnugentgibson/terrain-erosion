#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "cxxopts/cxxopts.h"
#include "graphics/image/binary/binimg.h"
#include "math/geometry/shapes/shape.h"

using graphics::image::OutputSpecifier;
using graphics::image::binary::Generate;
using graphics::image::binary::GeneratorFactory;
using math::geometry::shapes::CircleField;
using math::geometry::shapes::Shape;
using math::vector::Vector;

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0],
                           "converts a color image from binary to ppm");
  options.add_options()("w,width", "Width of image", cxxopts::value<int>())(
      "h,height", "Height of image", cxxopts::value<int>())(
      "o,output", "ppm image file path", cxxopts::value<std::string>());
  auto result = options.parse(argc, argv);
  int cols = result["w"].as<int>();
  int rows = result["h"].as<int>();
  std::stringstream fs;
  fs << result["o"].as<std::string>();
  fs << "_" << std::setfill('0') << std::setw(4) << rows;
  fs << "_" << std::setfill('0') << std::setw(4) << cols;
  fs << ".bin";
  std::string filename = fs.str();
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  auto builder = GeneratorFactory::get().Create("ShapeGenerator");
  CircleField field(0.05);
  field.Push(Vector(0.4, 0.4));
  field.Push(Vector(0.7, 0.3));
  field.Push(Vector(0.2, 0.1));
  field.Push(Vector(0.8, 0.7));
  builder->SetPtrParam("shape", &field);
  auto generator = (*builder)();
  Generate(OutputSpecifier(&ofs, rows, cols, sizeof(float)), generator.get());
  ofs.close();
  return 0;
}