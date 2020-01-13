#ifndef GRAPHICS_IMAGE_BINARY_BINIMG_REGISTRATION_H
#define GRAPHICS_IMAGE_BINARY_BINIMG_REGISTRATION_H

#include "graphics/image/binary/binimg.h"

namespace graphics {
namespace image {
namespace binary {
  
namespace GeneratorRegistrations {

template <typename T>
class GeneratorFactoryRegistration
{
public:
  GeneratorFactoryRegistration(const char* id)
  {
    GeneratorFactory::get().Register(
      id,
      []() { return std::unique_ptr<GeneratorBuilder>(new T()); }
    );
  }
};

} // namespace GeneratorRegistrations
  
namespace FunctorRegistrations {

template <typename T>
class FunctorFactoryRegistration
{
public:
  FunctorFactoryRegistration(const char* id)
  {
    FunctorFactory::get().Register(
      id,
      []() { return std::unique_ptr<FunctorBuilder>(new T()); }
    );
  }
};

} // namespace FunctorRegistrations
  
namespace TransformerRegistrations {

template <typename T>
class TransformerFactoryRegistration
{
public:
  TransformerFactoryRegistration(const char* id)
  {
    TransformerFactory::get().Register(
      id,
      []() { return std::unique_ptr<TransformerBuilder>(new T()); }
    );
  }
};

} // namespace TransformerRegistrations
  
namespace AccumulatorRegistrations {

template <typename T>
class AccumulatorFactoryRegistration
{
public:
  AccumulatorFactoryRegistration(const char* id)
  {
    AccumulatorFactory::get().Register(
      id,
      []() { return std::unique_ptr<AccumulatorBuilder>(new T()); }
    );
  }
};

} // namespace AccumulatorRegistrations
  
namespace CombinerRegistrations {

template <typename T>
class CombinerFactoryRegistration
{
public:
  CombinerFactoryRegistration(const char* id)
  {
    CombinerFactory::get().Register(
      id,
      []() { return std::unique_ptr<CombinerBuilder>(new T()); }
    );
  }
};

} // namespace CombinerRegistrations
  
namespace ColorizerRegistrations {

template <typename T>
class ColorizerFactoryRegistration
{
public:
  ColorizerFactoryRegistration(const char* id)
  {
    ColorizerFactory::get().Register(
      id,
      []() { return std::unique_ptr<ColorizerBuilder>(new T()); }
    );
  }
};

} // namespace ColorizerRegistrations

} // namespace binary
} // namespace image
} // namespace graphics

#endif // GRAPHICS_IMAGE_BINARY_BINIMG_REGISTRATION_H