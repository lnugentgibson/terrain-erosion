#ifndef GRAPHICS_MESH_IO_STL_H
#define GRAPHICS_MESH_IO_STL_H

#include <fstream>
#include <iostream>
#include <vector>

#include "math/vector/vector.h"

using math::vector::Vector;

namespace graphics {
namespace mesh {
namespace io {
namespace stl {

struct Triangle {
  Vector normal;
  Vector vertex1;
  Vector vertex2;
  Vector vertex3;
  Triangle() : normal(3), vertex1(3), vertex2(3), vertex3(3) {}
};

class STLData {
public:
  void Read(std::istream *is);

private:
  void ReadTriangle(std::istream *is);
  char header[80];
  std::vector<Triangle> triangles;
};

} // namespace stl
} // namespace io
} // namespace mesh
} // namespace graphics

#endif // GRAPHICS_MESH_IO_STL_H