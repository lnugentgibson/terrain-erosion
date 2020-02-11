#include "math/vector/vector.h"

#include <cmath>

#include "gtest/gtest.h"

using math::vector::Vector;

struct VectorParamsType {
  const float ax, ay, az, bx, by, bz, vsum_x, vsum_y, vsum_z, vdif_x, vdif_y,
      vdif_z, vprd_x, vprd_y, vprd_z, vquo_x, vquo_y, vquo_z, s, ssum_x, ssum_y,
      ssum_z, sdif_x, sdif_y, sdif_z, sprd_x, sprd_y, sprd_z, squo_x, squo_y,
      squo_z, d, l;
  Vector a, b;
  const Vector vsum, vdif, vprd, vquo, ssum, sdif, sprd, squo;
  const float scalar, dot, length;
  VectorParamsType(float _ax, float _ay, float _az, float _bx, float _by,
                   float _bz, float _vsum_x, float _vsum_y, float _vsum_z,
                   float _vdif_x, float _vdif_y, float _vdif_z, float _vprd_x,
                   float _vprd_y, float _vprd_z, float _vquo_x, float _vquo_y,
                   float _vquo_z, float _s, float _ssum_x, float _ssum_y,
                   float _ssum_z, float _sdif_x, float _sdif_y, float _sdif_z,
                   float _sprd_x, float _sprd_y, float _sprd_z, float _squo_x,
                   float _squo_y, float _squo_z, float _d, float _l)
      : ax(_ax), ay(_ay), az(_az), bx(_bx), by(_by), bz(_bz), vsum_x(_vsum_x),
        vsum_y(_vsum_y), vsum_z(_vsum_z), vdif_x(_vdif_x), vdif_y(_vdif_y),
        vdif_z(_vdif_z), vprd_x(_vprd_x), vprd_y(_vprd_y), vprd_z(_vprd_z),
        vquo_x(_vquo_x), vquo_y(_vquo_y), vquo_z(_vquo_z), s(_s),
        ssum_x(_ssum_x), ssum_y(_ssum_y), ssum_z(_ssum_z), sdif_x(_sdif_x),
        sdif_y(_sdif_y), sdif_z(_sdif_z), sprd_x(_sprd_x), sprd_y(_sprd_y),
        sprd_z(_sprd_z), squo_x(_squo_x), squo_y(_squo_y), squo_z(_squo_z),
        d(_d), l(_l), a(ax, ay, az), b(bx, by, bz),
        vsum(vsum_x, vsum_y, vsum_z), vdif(vdif_x, vdif_y, vdif_z),
        vprd(vprd_x, vprd_y, vprd_z), vquo(vquo_x, vquo_y, vquo_z),
        ssum(ssum_x, ssum_y, ssum_z), sdif(sdif_x, sdif_y, sdif_z),
        sprd(sprd_x, sprd_y, sprd_z), squo(squo_x, squo_y, squo_z), scalar(s),
        dot(d), length(l) {}
};

class VectorTest : public testing::TestWithParam<VectorParamsType> {
protected:
  VectorParamsType vals;
  VectorTest() : vals(GetParam()) {}
};

TEST_P(VectorTest, AIsValid) { EXPECT_TRUE(vals.a.valid()); }

TEST_P(VectorTest, AValue) {
  EXPECT_EQ(vals.a.d, 3);
  EXPECT_EQ(vals.a[0], vals.ax);
  EXPECT_EQ(vals.a[1], vals.ay);
  EXPECT_EQ(vals.a[2], vals.az);
}

TEST_P(VectorTest, BIsValid) { EXPECT_TRUE(vals.b.valid()); }

TEST_P(VectorTest, BValue) {
  EXPECT_EQ(vals.b.d, 3);
  EXPECT_EQ(vals.b[0], vals.bx);
  EXPECT_EQ(vals.b[1], vals.by);
  EXPECT_EQ(vals.b[2], vals.bz);
}

TEST_P(VectorTest, SSumIsValid) { EXPECT_TRUE(vals.ssum.valid()); }

TEST_P(VectorTest, SSumValue) {
  EXPECT_EQ(vals.ssum.d, 3);
  EXPECT_EQ(vals.ssum[0], vals.ssum_x);
  EXPECT_EQ(vals.ssum[1], vals.ssum_y);
  EXPECT_EQ(vals.ssum[2], vals.ssum_z);
}

TEST_P(VectorTest, AddScalar) {
  auto c = vals.a.add(vals.scalar);
  for (int i = 0; i < c.d; i++) {
    EXPECT_EQ(vals.ssum[i], c[i]);
  }
  // EXPECT_TRUE(vals.a.equals(vals.ssum));
  EXPECT_TRUE(c.equals(vals.ssum));
}

TEST_P(VectorTest, SubtractScalar) {
  auto c = vals.a.subtract(vals.scalar);
  EXPECT_TRUE(vals.a.equals(vals.sdif));
  EXPECT_TRUE(c.equals(vals.sdif));
}

TEST_P(VectorTest, MultiplyScalar) {
  auto c = vals.a.multiply(vals.scalar);
  EXPECT_TRUE(vals.a.equals(vals.sprd));
  EXPECT_TRUE(c.equals(vals.sprd));
}

TEST_P(VectorTest, DivideScalar) {
  auto c = vals.a.divide(vals.scalar);
  EXPECT_TRUE(vals.a.equals(vals.squo));
  EXPECT_TRUE(c.equals(vals.squo));
}

TEST_P(VectorTest, AddVector) {
  auto c = vals.a.add(vals.b);
  EXPECT_TRUE(vals.a.equals(vals.vsum));
  EXPECT_TRUE(c.HasValue());
  EXPECT_TRUE((*c).equals(vals.vsum));
}

TEST_P(VectorTest, SubtractVector) {
  auto c = vals.a.subtract(vals.b);
  EXPECT_TRUE(vals.a.equals(vals.vdif));
  EXPECT_TRUE(c.HasValue());
  EXPECT_TRUE((*c).equals(vals.vdif));
}

TEST_P(VectorTest, MultiplyVector) {
  auto c = vals.a.multiply(vals.b);
  EXPECT_TRUE(vals.a.equals(vals.vprd));
  EXPECT_TRUE(c.HasValue());
  EXPECT_TRUE((*c).equals(vals.vprd));
}

TEST_P(VectorTest, DivideVector) {
  auto c = vals.a.divide(vals.b);
  EXPECT_TRUE(vals.a.equals(vals.vquo));
  EXPECT_TRUE(c.HasValue());
  EXPECT_TRUE((*c).equals(vals.vquo));
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

INSTANTIATE_TEST_SUITE_P(Case1, VectorTest,
                         testing::Values(VectorParamsType(
                             // a
                             2, 3, 5,
                             // b
                             1, 2, 3,
                             // a + b
                             2 + 1.f, 3 + 2.f, 5 + 3.f,
                             // a - b
                             2 - 1.f, 3 - 2.f, 5 - 3.f,
                             // a * b
                             2 * 1.f, 3 * 2.f, 5 * 3.f,
                             // a / b
                             2 / 1.f, 3 / 2.f, 5 / 3.f,
                             // s
                             2.5f,
                             // a + s
                             2 + 2.5f, 3 + 2.5f, 5 + 2.5f,
                             // a - s
                             2 - 2.5f, 3 - 2.5f, 5 - 2.5f,
                             // a * s
                             2 * 2.5f, 3 * 2.5f, 5 * 2.5f,
                             // a / s
                             2 / 2.5f, 3 / 2.5f, 5 / 2.5f,
                             // a dot b
                             2 * 1.f + 3 * 2.f + 5 * 3.f,
                             // len(a)
                             sqrt(2 * 2 + 3 * 3 + 5 * 5))));

INSTANTIATE_TEST_SUITE_P(Case2, VectorTest,
                         testing::Values(VectorParamsType(
                             // a
                             3.14, 2.97, -1.63,
                             // b
                             -11.2, 6.75, 1.69,
                             // a + b
                             3.14 + -11.2, 2.97 + 6.75, -1.63 + 1.69,
                             // a - b
                             3.14 - -11.2, 2.97 - 6.75, -1.63 - 1.69,
                             // a * b
                             3.14 * -11.2, 2.97 * 6.75, -1.63 * 1.69,
                             // a / b
                             3.14 / -11.2, 2.97 / 6.75, -1.63 / 1.69,
                             // s
                             -7.31,
                             // a + s
                             3.14 + -7.31, 2.97 + -7.31, -1.63 + -7.31,
                             // a - s
                             3.14 - -7.31, 2.97 - -7.31, -1.63 - -7.31,
                             // a * s
                             3.14 * -7.31, 2.97 * -7.31, -1.63 * -7.31,
                             // a / s
                             3.14 / -7.31, 2.97 / -7.31, -1.63 / -7.31,
                             // a dot b
                             3.14 * -11.2 + 2.97 * 6.75 + -1.63 * 1.69,
                             // len(a)
                             sqrt(3.14 * 3.14 + 2.97 * 2.97 +
                                  (-1.63) * (-1.63)))));

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}