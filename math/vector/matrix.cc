#include "math/vector/matrix.h"

#include <cmath>
#include <sstream>

#include "math/common.h"

/*
bool Matrix::equals(const Vector& v, float tolerance) const {
  return reduce([v, tolerance](bool a, float e, int i, float *A) { return a && floatEquals(e, v[i], tolerance); }, true);
}
//*/
std::string Matrix::toStr() const {
  std::stringstream ss;
  ss << "Matrix" << m << 'x' << n << '[';
  ss << c[0];
  for(int i = 1; i < n; i++) {
    ss << ',' << c[i];
  }
  for(int j = 1; j < m; j++) {
    ss << ';' << c[j * n];
    for(int i = 1; i < n; i++) {
      ss << ',' << c[j * n + i];
    }
  }
  ss << ']';
  return ss.str();
}