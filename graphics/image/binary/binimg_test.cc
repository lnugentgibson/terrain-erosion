#include "gtest/gtest.h"

#include <sstream>
#include <string>

#include "graphics/image/binary/binimg.h"

using graphics::image::binary::DataSpecifier;
using graphics::image::binary::InputSpecifier;
using graphics::image::binary::Neighborhood;

class NeighborhoodTest;

class NeighborhoodProxy : public Neighborhood {
  friend NeighborhoodTest;
 public:
  NeighborhoodProxy(int _span, int _cols, DataSpecifier _in_spec) : Neighborhood(_span, _cols, _in_spec) {}
  void SetCenter(int i, int j) {
    center_i = i;
    center_j = j;
  }
  void PushBack() {
    buffer.push_back(new char[cols]);
  }
  void Set(int i, int j, char v) {
    buffer[i][j] = v;
  }
};

class NeighborhoodTest : public testing::Test {
 protected:
  void SetCenter(NeighborhoodProxy n, int i, int j) {
    n.center_i = i;
    n.center_j = j;
  }
  void PushBack(NeighborhoodProxy n, int cols) {
    n.buffer.push_back(new char[cols]);
  }
  void Set(NeighborhoodProxy n, int i, int j, char v) {
    n.buffer[i][j] = v;
  }
};

TEST_F(NeighborhoodTest, RangeOnCornerNN) {
  int span = 2, cols = 16;
  NeighborhoodProxy n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(0, 0);
  for(int i = 0; i <= span; i++) {
    n.PushBack();
    for(int j = 0; j < cols; j++) {
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
  for(int i = r[0]; i < r[1]; i++) {
    for(int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

TEST_F(NeighborhoodTest, RangeNearCornerNN) {
  int span = 2, cols = 16;
  NeighborhoodProxy n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(span / 2, span / 2);
  for(int i = -span / 2; i <= span; i++) {
    n.PushBack();
    for(int j = 0; j < cols; j++) {
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
  for(int i = r[0]; i < r[1]; i++) {
    for(int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

TEST_F(NeighborhoodTest, RangeMiddle) {
  int span = 2, cols = 16;
  NeighborhoodProxy n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(span, cols / 2);
  for(int i = -span; i <= span; i++) {
    n.PushBack();
    for(int j = 0; j < cols; j++) {
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
  for(int i = r[0]; i < r[1]; i++) {
    for(int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

TEST_F(NeighborhoodTest, RangeNearCornerPP) {
  int span = 2, cols = 16;
  NeighborhoodProxy n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(span, cols - span / 2 - 1);
  for(int i = -span; i <= span / 2; i++) {
    n.PushBack();
    for(int j = 0; j < cols; j++) {
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
  for(int i = r[0]; i < r[1]; i++) {
    for(int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}

TEST_F(NeighborhoodTest, RangeOnCornerPP) {
  int span = 2, cols = 16;
  NeighborhoodProxy n(span, cols, DataSpecifier(sizeof(char)));
  n.SetCenter(span, cols - 1);
  for(int i = -span; i <= 0; i++) {
    n.PushBack();
    for(int j = 0; j < cols; j++) {
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
  for(int i = r[0]; i < r[1]; i++) {
    for(int j = r[2]; j < r[3]; j++) {
      p = n.get(i, j);
      EXPECT_EQ(p.pixel[0], base + i * cols + j);
    }
  }
}
 
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}