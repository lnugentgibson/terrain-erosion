#include "math/geometry/shapes/shape.h"
#include "graphics/image/binary/binimg.h"
#include "graphics/image/binary/binimg_registration.h"

using graphics::image::PixelSpecifier;
using graphics::image::binary::Generator;
using graphics::image::binary::GeneratorBuilder;
using graphics::image::binary::SimpleTransformer;
using graphics::image::binary::StatelessGenerator;
using graphics::image::binary::Transformer;
using graphics::image::binary::TransformerBuilder;
using math::geometry::shapes::Shape;
using math::vector::Vector;

class ShapeGenerator : public StatelessGenerator {
  Shape *shape;

public:
  ShapeGenerator(Shape *s) : shape(s) {}
  void Generate(int i, int j, int rows, int cols,
                PixelSpecifier pixel) override;
};

void ShapeGenerator::Generate(int i, int j, int rows, int cols,
                              PixelSpecifier pixel) {
  Vector p(2);
  p[0] = static_cast<float>(j) / cols;
  p[1] = static_cast<float>(i) / rows;
  *reinterpret_cast<float *>(pixel.pixel) = shape->Inside(p) ? 1.0 : 0.0;
}

class ShapeGeneratorBuilder : public GeneratorBuilder {
  Shape *shape;

public:
  std::unique_ptr<Generator> operator()() override {
    return std::unique_ptr<Generator>(new ShapeGenerator(shape));
  }
  bool SetIntParam(const std::string &param, int value) override {
    return false;
  }
  bool SetFloatParam(const std::string &param, float value) override {
    return false;
  }
  bool SetPtrParam(const std::string &param, void *value) override;
};

bool ShapeGeneratorBuilder::SetPtrParam(const std::string &param, void *value) {
  if (param == "shape") {
    shape = static_cast<Shape *>(value);
    return true;
  }
  return false;
}

namespace graphics {
namespace image {
namespace binary {
namespace GeneratorRegistrations {
GeneratorFactoryRegistration<ShapeGeneratorBuilder>
    _ShapeGeneratorBuilder("ShapeGenerator");
}
} // namespace binary
} // namespace image
} // namespace graphics

class ShapeTransformer : public SimpleTransformer {
  Shape *shape;

public:
  ShapeTransformer(Shape *s) : shape(s) {}
  void Transform(int i, int j, int rows, int cols,
                 const PixelSpecifier in_pixel,
                 PixelSpecifier out_pixel) override;
};

void ShapeTransformer::Transform(int i, int j, int rows, int cols,
                                 const PixelSpecifier in_pixel,
                                 PixelSpecifier out_pixel) {
  Vector p(2);
  p[0] = static_cast<float>(j) / cols;
  p[1] = static_cast<float>(i) / rows;
  *reinterpret_cast<float *>(out_pixel.pixel) =
      shape->Inside(p) ? 1.0 : *reinterpret_cast<float *>(in_pixel.pixel);
}

class ShapeTransformerBuilder : public TransformerBuilder {
  Shape *shape;

public:
  std::unique_ptr<Transformer> operator()() override {
    return std::unique_ptr<Transformer>(new ShapeTransformer(shape));
  }
  bool SetIntParam(const std::string &param, int value) override {
    return false;
  }
  bool SetFloatParam(const std::string &param, float value) override {
    return false;
  }
  bool SetPtrParam(const std::string &param, void *value) override;
};

bool ShapeTransformerBuilder::SetPtrParam(const std::string &param,
                                          void *value) {
  if (param == "shape") {
    shape = static_cast<Shape *>(value);
    return true;
  }
  return false;
}

namespace graphics {
namespace image {
namespace binary {
namespace TransformerRegistrations {
TransformerFactoryRegistration<ShapeTransformerBuilder>
    _ShapeTransformerBuilder("ShapeTransformer");
}
} // namespace binary
} // namespace image
} // namespace graphics