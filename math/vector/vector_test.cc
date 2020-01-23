#include "math/vector/vector.h"

#include <cmath>

#include "gtest/gtest.h"

using math::vector::Vector;

struct VectorParamsType {
  Vector a, b,
    vsum, vdif, vprd, vquo,
    ssum, sdif, sprd, squo;
  float scalar, dot, length;
  VectorParamsType(
    float ax, float ay, float az,
    float bx, float by, float bz,
    float vsum_x, float vsum_y, float vsum_z,
    float vdif_x, float vdif_y, float vdif_z,
    float vprd_x, float vprd_y, float vprd_z,
    float vquo_x, float vquo_y, float vquo_z,
    float s,
    float ssum_x, float ssum_y, float ssum_z,
    float sdif_x, float sdif_y, float sdif_z,
    float sprd_x, float sprd_y, float sprd_z,
    float squo_x, float squo_y, float squo_z,
    float d,
    float l
  ) : a(ax, ay, az), b(bx, by, bz),
    vsum(vsum_x, vsum_y, vsum_z),
    vdif(vdif_x, vdif_y, vdif_z),
    vprd(vprd_x, vprd_y, vprd_z),
    vquo(vquo_x, vquo_y, vquo_z),
    ssum(ssum_x, ssum_y, ssum_z),
    sdif(sdif_x, sdif_y, sdif_z),
    sprd(sprd_x, sprd_y, sprd_z),
    squo(squo_x, squo_y, squo_z),
    scalar(s), dot(d), length(l) {}
};

class VectorTest : public testing::TestWithParam<VectorParamsType> {
 protected:
  VectorParamsType vals;
  VectorTest() : vals(GetParam()) {}
};

TEST_P(VectorTest, AddScalar) {
  auto c = vals.a.add(vals.scalar);
  EXPECT_EQ(c, vals.ssum);
}

TEST_P(VectorTest, SubtractScalar) {
  auto c = vals.a.subtract(vals.scalar);
  EXPECT_EQ(c, vals.sdif);
}

TEST_P(VectorTest, MultiplyScalar) {
  auto c = vals.a.multiply(vals.scalar);
  EXPECT_EQ(c, vals.sprd);
}

TEST_P(VectorTest, DivideScalar) {
  auto c = vals.a.divide(vals.scalar);
  EXPECT_EQ(c, vals.squo);
}

TEST_P(VectorTest, AddVector) {
  auto c = vals.a.add(vals.b);
  EXPECT_TRUE(c.HasValue());
  EXPECT_EQ(*c, vals.vsum);
}

TEST_P(VectorTest, SubtractVector) {
  auto c = vals.a.subtract(vals.b);
  EXPECT_TRUE(c.HasValue());
  EXPECT_EQ(*c, vals.vdif);
}

TEST_P(VectorTest, MultiplyVector) {
  auto c = vals.a.multiply(vals.b);
  EXPECT_TRUE(c.HasValue());
  EXPECT_EQ(*c, vals.vprd);
}

TEST_P(VectorTest, DivideVector) {
  auto c = vals.a.divide(vals.b);
  EXPECT_TRUE(c.HasValue());
  EXPECT_EQ(*c, vals.vquo);
}

TEST_P(VectorTest, Dot) {
  auto d = vals.a.dot(vals.b);
  EXPECT_TRUE(d.HasValue());
  ASSERT_FLOAT_EQ(*d, vals.dot);
}

TEST_P(VectorTest, Length) {
  auto l = vals.a.length();
  ASSERT_FLOAT_EQ(l, vals.length);
}

INSTANTIATE_TEST_SUITE_P(Case1,
                         VectorTest,
                         testing::Values(VectorParamsType(
                           2, 3, 5,
                           1, 2, 3,
                           2 + 1.f, 3 + 2.f, 5 + 3.f,
                           2 - 1.f, 3 - 2.f, 5 - 3.f,
                           2 * 1.f, 3 * 2.f, 5 * 3.f,
                           2 / 1.f, 3 / 2.f, 5 / 3.f,
                           2.5f,
                           2 + 2.5f, 3 + 2.5f, 5 + 2.5f,
                           2 - 2.5f, 3 - 2.5f, 5 - 2.5f,
                           2 * 2.5f, 3 * 2.5f, 5 * 2.5f,
                           2 / 2.5f, 3 / 2.5f, 5 / 2.5f,
                           2 * 1.f + 3 * 2.f + 5 * 3.f,
                           sqrt(2*2 + 3*3 + 5*5)
                         )));

INSTANTIATE_TEST_SUITE_P(Case2,
                         VectorTest,
                         testing::Values(VectorParamsType(
                           3.14, 2.97, -1.63,
                           -11.2, 6.75, 1.69,
                           3.14 + -11.2, 2.97 + 6.75, -1.63 + 1.69,
                           3.14 - -11.2, 2.97 - 6.75, -1.63 - 1.69,
                           3.14 * -11.2, 2.97 * 6.75, -1.63 * 1.69,
                           3.14 / -11.2, 2.97 / 6.75, -1.63 / 1.69,
                           -7.31,
                           3.14 + -7.31, 2.97 + -7.31, -1.63 + -7.31,
                           3.14 - -7.31, 2.97 - -7.31, -1.63 - -7.31,
                           3.14 * -7.31, 2.97 * -7.31, -1.63 * -7.31,
                           3.14 / -7.31, 2.97 / -7.31, -1.63 / -7.31,
                           3.14 * -11.2 + 2.97 * 6.75 + -1.63 * 1.69,
                           sqrt(3.14*3.14 + 2.97*2.97 + (-1.63)*(-1.63))
                         )));
 
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}