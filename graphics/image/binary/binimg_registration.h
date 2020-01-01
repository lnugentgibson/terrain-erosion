#ifndef BINIMG_REGISTRATION_H
#define BINIMG_REGISTRATION_H

#include "graphics/image/binary/binimg.h"

namespace graphics {
namespace image {
namespace binary {
  
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

} // namespace binary

} // namespace binary
} // namespace image
} // namespace graphics

#endif // BINIMG_REGISTRATION_H