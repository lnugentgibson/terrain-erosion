#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "cxxopts/cxxopts.h"
#include "math/geometry/shapes/shape.h"
#include "graphics/image/binary/binimg.h"

using graphics::image::binary::Generate;
using graphics::image::binary::GeneratorFactory;
using graphics::image::binary::OutputSpecifier;
using math::geometry::shapes::Circle;
using math::geometry::shapes::Rectangle;
using math::geometry::shapes::Shape;
using math::vector::Vector;

int main(int argc, char *argv[]) {
  cxxopts::Options options(argv[0], "converts a color image from binary to ppm");
  options.add_options()
    ("w,width", "Width of image", cxxopts::value<int>())
    ("h,height", "Height of image", cxxopts::value<int>())
    ("o,output", "ppm image file path", cxxopts::value<std::string>())
    ;
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
  // builder->SetPtrParam("shape", new Rectangle(Vector(0.25, 0.25), Vector(0.5, 0.5)));
  builder->SetPtrParam("shape", new Circle(Vector(0.4, 0.4), 0.1));
  auto generator = (*builder)();
  Generate(rows, cols, OutputSpecifier(ofs, sizeof(float)), generator.get());
  ofs.close();
  return 0;
}