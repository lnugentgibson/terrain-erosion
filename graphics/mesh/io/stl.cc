#include "graphics/mesh/io/stl.h"

namespace graphics {
namespace mesh {
namespace io {
namespace stl {

void STLData::Read(std::istream *is) {
  is->read(header, 80);
  int32_t n;
  is->read((char *)&n, 4);
  for (int i = 0; i < n; i++) {
    ReadTriangle(is);
  }
}

void ReadVector(Vector v, std::istream *is) {
  float c[3];
  is->read((char *)c, 3 * sizeof(float));
  v.set(c);
}

void STLData::ReadTriangle(std::istream *is) {
  Triangle tri;
  ReadVector(tri.normal, is);
  ReadVector(tri.vertex1, is);
  ReadVector(tri.vertex2, is);
  ReadVector(tri.vertex3, is);
  triangles.push_back(tri);
}

} // namespace stl
} // namespace io
} // namespace mesh
} // namespace graphics