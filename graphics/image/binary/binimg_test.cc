#include "gtest/gtest.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "graphics/image/binary/binimg.h"

namespace graphics {
namespace image {
namespace binary {

namespace {

TEST(NeighborhoodTest, RangeOnCornerNN) {
  int span = 2, cols = 16;
  Neighborhood n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(0, 0);
  for (int i = 0; i <= span; i++) {
    n.Push();
    for (int j = 0; j < cols; j++) {
      n.Set(i, j, i * cols + j);
    }
  }
  auto r = n.range();
  EXPECT_EQ(r[0], 0);
  EXPECT_EQ(r[1], span + 1);
  EXPECT_EQ(r[2], 0);
  EXPECT_EQ(r[3], span + 1);
  auto p = n.get(0, 0);
  char base = 0;
  EXPECT_EQ(p.pixel[0], base);
  for (int i = r[0]; i < r[1]; i++) {
    for (int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

TEST(NeighborhoodTest, RangeNearCornerNN) {
  int span = 2, cols = 16;
  Neighborhood n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(span / 2, span / 2);
  for (int i = -span / 2; i <= span; i++) {
    n.Push();
    for (int j = 0; j < cols; j++) {
      n.Set(i + span / 2, j, (i + span / 2) * cols + j);
    }
  }
  auto r = n.range();
  EXPECT_EQ(r[0], -span / 2);
  EXPECT_EQ(r[1], span + 1);
  EXPECT_EQ(r[2], -span / 2);
  EXPECT_EQ(r[3], span + 1);
  auto p = n.get(0, 0);
  char base = (span / 2) * cols + span / 2;
  EXPECT_EQ(p.pixel[0], base);
  for (int i = r[0]; i < r[1]; i++) {
    for (int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

TEST(NeighborhoodTest, RangeMiddle) {
  int span = 2, cols = 16;
  Neighborhood n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(span, cols / 2);
  for (int i = -span; i <= span; i++) {
    n.Push();
    for (int j = 0; j < cols; j++) {
      n.Set(i + span, j, (i + span) * cols + j);
    }
  }
  auto r = n.range();
  EXPECT_EQ(r[0], -span);
  EXPECT_EQ(r[1], span + 1);
  EXPECT_EQ(r[2], -span);
  EXPECT_EQ(r[3], span + 1);
  auto p = n.get(0, 0);
  char base = span * cols + cols / 2;
  EXPECT_EQ(p.pixel[0], base);
  for (int i = r[0]; i < r[1]; i++) {
    for (int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

TEST(NeighborhoodTest, RangeNearCornerPP) {
  int span = 2, cols = 16;
  Neighborhood n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(span, cols - span / 2 - 1);
  for (int i = -span; i <= span / 2; i++) {
    n.Push();
    for (int j = 0; j < cols; j++) {
      n.Set(i + span, j, (i + span) * cols + j);
    }
  }
  auto r = n.range();
  EXPECT_EQ(r[0], -span);
  EXPECT_EQ(r[1], span / 2 + 1);
  EXPECT_EQ(r[2], -span);
  EXPECT_EQ(r[3], span / 2 + 1);
  auto p = n.get(0, 0);
  char base = span * cols + cols - span / 2 - 1;
  EXPECT_EQ(p.pixel[0], base);
  for (int i = r[0]; i < r[1]; i++) {
    for (int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

TEST(NeighborhoodTest, RangeOnCornerPP) {
  int span = 2, cols = 16;
  Neighborhood n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(span, cols - 1);
  for (int i = -span; i <= 0; i++) {
    n.Push();
    for (int j = 0; j < cols; j++) {
      n.Set(i + span, j, (i + span) * cols + j);
    }
  }
  auto r = n.range();
  EXPECT_EQ(r[0], -span);
  EXPECT_EQ(r[1], 1);
  EXPECT_EQ(r[2], -span);
  EXPECT_EQ(r[3], 1);
  auto p = n.get(0, 0);
  char base = span * cols + cols - 1;
  EXPECT_EQ(p.pixel[0], base);
  for (int i = r[0]; i < r[1]; i++) {
    for (int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

class MockNeighborhoodTransformer : public Transformer {
private:
  int span, rows, cols;
  int callCount;

public:
  MockNeighborhoodTransformer(int s, int r, int c)
      : span(s), rows(r), cols(c), callCount(0) {}
  inline int CallCount() { return callCount; }

  void Transform(int i, int j, int rows, int cols,
                 const PixelSpecifier in_pixel,
                 PixelSpecifier out_pixel) override {}

  void TransformStateful(int i, int j, int rows, int cols,
                         const PixelSpecifier in_pixel,
                         PixelSpecifier out_pixel, void *state) override {}

  void TransformNeighborhood(int i, int j, int rows, int cols,
                             const Neighborhood &neighborhood,
                             PixelSpecifier out_pixel) override {
    callCount++;
    auto r = neighborhood.range();
    if (i < span) {
      EXPECT_EQ(r[0], -i) << "Range at pixel " << i << ", " << j
                          << " has incorrect lower bound for row";
    } else {
      EXPECT_EQ(r[0], -span) << "Range at pixel " << i << ", " << j
                             << " has incorrect lower bound for row";
    }
    if (rows - i <= span) {
      EXPECT_EQ(r[1], rows - i) << "Range at pixel " << i << ", " << j
                                << " has incorrect upper bound for row";
    } else {
      EXPECT_EQ(r[1], span + 1) << "Range at pixel " << i << ", " << j
                                << " has incorrect upper bound for row";
    }
    if (j < span) {
      EXPECT_EQ(r[2], -j) << "Range at pixel " << i << ", " << j
                          << " has incorrect lower bound for col";
    } else {
      EXPECT_EQ(r[2], -span) << "Range at pixel " << i << ", " << j
                             << " has incorrect lower bound for col";
    }
    if (cols - j <= span) {
      EXPECT_EQ(r[3], cols - j) << "Range at pixel " << i << ", " << j
                                << " has incorrect upper bound for col";
    } else {
      EXPECT_EQ(r[3], span + 1) << "Range at pixel " << i << ", " << j
                                << " has incorrect upper bound for col";
    }
    auto p = neighborhood.get(0, 0);
    char base = (char)(i * cols + j);
    EXPECT_EQ(p.pixel[0], base)
        << "Value of pixel " << i << ", " << j << " is incorrect";
    for (int i_ = r[0]; i_ < r[1]; i_++) {
      for (int j_ = r[2]; j_ < r[3]; j_++) {
        p = neighborhood.get(i_, j_);
        EXPECT_EQ(p.pixel[0], (char)(base + i_ * cols + j_))
            << "Value at (" << i_ << ", " << j_ << ") from pixel " << i << ", "
            << j << " is incorrect";
      }
    }
  }
};

TEST(MapNeighborhoodTest, Test) {
  // Create data
  int span = 4;
  const int header_size = sizeof(int) * 3;
  char data[header_size + 144 + 1];
  int *header = reinterpret_cast<int *>(data);
  header[0] = 12;
  header[1] = 12;
  header[2] = 1;
  for (int i = 0; i < 144; i++)
    data[header_size + i] = (char)i;
  data[header_size + 144] = 0;
  std::string str(std::begin(data), std::end(data));
  std::istringstream is(str);
  std::ostringstream os;
  DataSpecifier spec(sizeof(char));
  InputSpecifier in_spec(static_cast<std::istream *>(&is), spec);
  OutputSpecifier out_spec(static_cast<std::ostream *>(&os), spec);

  // Testing data and stream
  in_spec.read();
  int &rows = in_spec.data.rows;
  int &cols = in_spec.data.cols;
  int &dim = in_spec.data.data.dim;
  EXPECT_EQ(rows, 12) << "rows not equal to 12";
  EXPECT_EQ(header[0], 12) << "rows not equal to 12";
  EXPECT_EQ(cols, 12) << "cols not equal to 12";
  EXPECT_EQ(header[1], 12) << "cols not equal to 12";
  EXPECT_EQ(dim, 1) << "dim not equal to 1";
  EXPECT_EQ(header[2], 1) << "dim not equal to 1";
  PixelSpecifier pixel(spec);
  for (int i = 0; i < 144; i++) {
    pixel.read(in_spec);
    EXPECT_EQ(pixel.pixel[0], (char)i)
        << "pixel at index " << i << " does not equal " << i;
  }
  is.seekg(0, is.beg);

  // Setup Transformer
  MockNeighborhoodTransformer transformer(span, rows, cols);
  MapNeighborhood(in_spec, out_spec, span, &transformer);

  EXPECT_EQ(transformer.CallCount(), 144);
}

} // namespace

} // namespace binary
} // namespace image
} // namespace graphics

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}