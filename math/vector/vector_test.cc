#include "math/vector/vector.h"

#include <cmath>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace math {
namespace vector {

namespace {

MATCHER_P(VectorEq, v, "") {
  if (arg.dimension() != v.dimension()) {
    *result_listener << "do not have the same dimension: " << arg.dimension()
                     << " <> " << v.dimension();
  }
  auto result = arg.equals(v);
  if (!result) {
    *result_listener << "elements differ: [" << arg.toStr() << "] <> ["
                     << v.toStr() << "]";
  }
  return result;
}

class FVectorTest : public testing::TestWithParam<int> {
protected:
  const int dimension_;
  FVectorTest() : dimension_(GetParam()) {}
};

INSTANTIATE_TEST_SUITE_P(Dim2, FVectorTest, testing::Values(2));
INSTANTIATE_TEST_SUITE_P(Dim3, FVectorTest, testing::Values(3));
INSTANTIATE_TEST_SUITE_P(Dim4, FVectorTest, testing::Values(4));

TEST_P(FVectorTest, ConstructorDim) {
  FVector v(dimension_);
  EXPECT_EQ(v.dimension(), dimension_);
  for (int i = 0; i < dimension_; i++) {
    EXPECT_THAT(v[i], testing::FloatEq(0));
  }
}

TEST_P(FVectorTest, ConstructorPtr) {
  float cs[4];
  for (int i = 0; i < dimension_; i++) {
    cs[i] = i + 1;
  }
  FVector v(dimension_, cs);
  EXPECT_EQ(v.dimension(), dimension_);
  for (int i = 0; i < dimension_; i++) {
    EXPECT_THAT(v[i], testing::FloatEq(i + 1));
  }
}

TEST_P(FVectorTest, ConstructorIntPtr) {
  int cs[4];
  for (int i = 0; i < dimension_; i++) {
    cs[i] = i + 1;
  }
  FVector v(dimension_, cs);
  EXPECT_EQ(v.dimension(), dimension_);
  for (int i = 0; i < dimension_; i++) {
    EXPECT_THAT(v[i], testing::FloatEq(i + 1));
  }
}

TEST(FVectorTest, ConstructorParams2D) {
  FVector v(1.0, 2.0);
  EXPECT_EQ(v.dimension(), 2);
  EXPECT_THAT(v[0], testing::FloatEq(1.0));
  EXPECT_THAT(v[1], testing::FloatEq(2.0));
}

TEST(FVectorTest, ConstructorParams3D) {
  FVector v(1.0, 2.0, 3.0);
  EXPECT_EQ(v.dimension(), 3);
  EXPECT_THAT(v[0], testing::FloatEq(1.0));
  EXPECT_THAT(v[1], testing::FloatEq(2.0));
  EXPECT_THAT(v[2], testing::FloatEq(3.0));
}

TEST(FVectorTest, ConstructorParams4D) {
  FVector v(1.0, 2.0, 3.0, 4.0);
  EXPECT_EQ(v.dimension(), 4);
  EXPECT_THAT(v[0], testing::FloatEq(1.0));
  EXPECT_THAT(v[1], testing::FloatEq(2.0));
  EXPECT_THAT(v[2], testing::FloatEq(3.0));
  EXPECT_THAT(v[3], testing::FloatEq(4.0));
}

TEST_P(FVectorTest, ConstructorRef) {
  FVector p(dimension_);
  for (int i = 0; i < dimension_; i++) {
    p[i] = i + 1;
  }
  FVector v(p);
  EXPECT_EQ(v.dimension(), dimension_);
  for (int i = 0; i < dimension_; i++) {
    EXPECT_THAT(v[i], testing::FloatEq(i + 1));
  }
}

/*
FVector&& RRef(int d) {
  FVector v(d);
  EXPECT_EQ(v.dimension(), d);
  for (int i = 0; i < d; i++) {
    v[i] = i + 1;
  }
  for (int i = 0; i < d; i++) {
    EXPECT_THAT(v[i], testing::FloatEq(i + 1));
  }
  return std::move(v);
}

TEST(FVectorTest, ConstructorRRef2D) {
  FVector&& p = std::move(RRef(2));
  // EXPECT_EQ(p.dimension(), 2);
  // EXPECT_THAT(p[0], testing::FloatEq(1.0));
  // EXPECT_THAT(p[1], testing::FloatEq(2.0));
  FVector v(std::move(p));
  EXPECT_EQ(v.dimension(), 2);
  EXPECT_THAT(v[0], testing::FloatEq(1.0));
  EXPECT_THAT(v[1], testing::FloatEq(2.0));
}

TEST(FVectorTest, ConstructorRRef3D) {
  FVector&& p = RRef(3);
  FVector v(std::move(p));
  EXPECT_EQ(v.dimension(), 3);
  EXPECT_THAT(v[0], testing::FloatEq(1.0));
  EXPECT_THAT(v[1], testing::FloatEq(2.0));
  EXPECT_THAT(v[2], testing::FloatEq(3.0));
}

TEST(FVectorTest, ConstructorRRef4D) {
  FVector&& p = RRef(4);
  FVector v(std::move(p));
  EXPECT_EQ(v.dimension(), 4);
  EXPECT_THAT(v[0], testing::FloatEq(1.0));
  EXPECT_THAT(v[1], testing::FloatEq(2.0));
  EXPECT_THAT(v[2], testing::FloatEq(3.0));
  EXPECT_THAT(v[3], testing::FloatEq(4.0));
}
*/

TEST_P(FVectorTest, ConstructorIRef3D) {
  IVector p(dimension_);
  for (int i = 0; i < dimension_; i++) {
    p[i] = i + 1;
  }
  FVector v(p);
  EXPECT_EQ(v.dimension(), dimension_);
  for (int i = 0; i < dimension_; i++) {
    EXPECT_THAT(v[i], testing::FloatEq(i + 1));
  }
}

template <typename VectorType = FVector, typename DataType = float,
          typename FloatType = float>
struct Quadruplet {
  DataType x, y, z, w;
  FloatType l[3];
  Quadruplet(DataType x_, DataType y_, DataType z_, DataType w_)
      : x(x_), y(y_), z(z_), w(w_) {
    l[0] = sqrt(x * x + y * y);
    l[1] = sqrt(x * x + y * y + z * z);
    l[2] = sqrt(x * x + y * y + z * z + w * w);
  }
  Quadruplet(const Quadruplet<VectorType, DataType, FloatType> &t) = default;
  VectorType vec(int d = 3) const {
    switch (d) {
    case 2:
      return VectorType(x, y);
    case 3:
      return VectorType(x, y, z);
    case 4:
      return VectorType(x, y, z, w);
    default:
      return VectorType(d);
    }
  }
};

template <typename VectorType = FVector, typename DataType = float,
          typename FloatType = float>
struct QuadrupletUnaryArith {
  Quadruplet<VectorType, DataType, FloatType> neg, sum, dif, prd, quo;

  QuadrupletUnaryArith(DataType s,
                       const Quadruplet<VectorType, DataType, FloatType> &v)
      : neg(-v.x, -v.y, -v.z, -v.w), sum(v.x + s, v.y + s, v.z + s, v.w + s),
        dif(v.x - s, v.y - s, v.z - s, v.w - s),
        prd(v.x * s, v.y * s, v.z * s, v.w * s),
        quo(v.x / s, v.y / s, v.z / s, v.w / s) {}
  QuadrupletUnaryArith(const QuadrupletUnaryArith &o) = default;
};

template <typename VectorType = FVector, typename IntVectorType = IVector,
          typename DataType = float, typename FloatType = float,
          typename IntType = int>
struct QuadrupletUnaryFC {
  Quadruplet<VectorType, DataType, FloatType> floor_i, ceil_i;
  Quadruplet<IntVectorType, IntType, FloatType> floor, ceil;

  explicit QuadrupletUnaryFC(
      const Quadruplet<VectorType, DataType, FloatType> &v)
      : floor_i(::floor(v.x), ::floor(v.y), ::floor(v.z), ::floor(v.w)),
        ceil_i(::ceil(v.x), ::ceil(v.y), ::ceil(v.z), ::ceil(v.w)),
        floor(::floor(v.x), ::floor(v.y), ::floor(v.z), ::floor(v.w)),
        ceil(::ceil(v.x), ::ceil(v.y), ::ceil(v.z), ::ceil(v.w)) {}
  QuadrupletUnaryFC(const QuadrupletUnaryFC &o) = default;
};

template <typename VectorType = FVector, typename DataType = float,
          typename FloatType = float>
struct QuadrupletBinary {
  Quadruplet<VectorType, DataType, FloatType> sum, dif, prd, quo;

  float d[3];
  QuadrupletBinary(const Quadruplet<VectorType, DataType, FloatType> &a,
                   const Quadruplet<VectorType, DataType, FloatType> &b)
      : sum(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w),
        dif(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w),
        prd(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w),
        quo(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w) {
    d[0] = a.x * b.x + a.y * b.y;
    d[1] = a.x * b.x + a.y * b.y + a.z * b.z;
    d[2] = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }
  QuadrupletBinary(const QuadrupletBinary &o) = default;
};

template <typename VectorType = FVector, typename DataType = float,
          typename FloatType = float>
struct VectorParamsTypeArith {
  const Quadruplet<VectorType, DataType, FloatType> as, bs;
  const QuadrupletUnaryArith<VectorType, DataType, FloatType> au, bu;
  const QuadrupletBinary<VectorType, DataType, FloatType> ab;
  std::vector<VectorType> a, b;
  const float scalar;
  VectorParamsTypeArith(const Quadruplet<VectorType, DataType, FloatType> &_a,
                        const Quadruplet<VectorType, DataType, FloatType> &_b,
                        float _s)
      : as(_a), bs(_b), au(_s, _a), bu(_s, _b), ab(_a, _b), scalar(_s) {
    a.emplace_back(as.x, as.y);
    a.emplace_back(as.x, as.y, as.z);
    a.emplace_back(as.x, as.y, as.z, as.w);
    b.emplace_back(bs.x, bs.y);
    b.emplace_back(bs.x, bs.y, bs.z);
    b.emplace_back(bs.x, bs.y, bs.z, bs.w);
  }
};

class IVectorArithmeticTest : public testing::TestWithParam<
                                  VectorParamsTypeArith<IVector, int, float>> {
protected:
  VectorParamsTypeArith<IVector, int, float> vals_;
  IVectorArithmeticTest() : vals_(GetParam()) {}
};

class FVectorArithmeticTest
    : public testing::TestWithParam<VectorParamsTypeArith<>> {
protected:
  VectorParamsTypeArith<> vals_;
  FVectorArithmeticTest() : vals_(GetParam()) {}
};

class DVectorArithmeticTest
    : public testing::TestWithParam<
          VectorParamsTypeArith<DVector, double, double>> {
protected:
  VectorParamsTypeArith<DVector, double, double> vals_;
  DVectorArithmeticTest() : vals_(GetParam()) {}
};

#define VECTOR_UNARY_OP_INPLACE_SCALAR_TEST(Suite, TestName, opName, op,       \
                                            expected)                          \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto &v = vals_.a[i];                                                    \
      auto &a = v.opName();                                                    \
      auto e = vals_.au.expected.vec(d);                                       \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Eq(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      v = vals_.b[i];                                                          \
      a = v.opName();                                                          \
      e = vals_.bu.expected.vec(d);                                            \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Eq(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

VECTOR_UNARY_OP_INPLACE_SCALAR_TEST(FVectorArithmeticTest, NegateTest, negate,
                                    -, neg)

#define VECTOR_UNARY_OP_OUTOFPLACE_SCALAR_TEST(Suite, TestName, opName, op,    \
                                               expected)                       \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto v = vals_.a[i];                                                     \
      auto a = v.opName();                                                     \
      auto e = vals_.au.expected.vec(d);                                       \
      EXPECT_EQ(v.dimension(), e.dimension());                                 \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Ne(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

VECTOR_UNARY_OP_OUTOFPLACE_SCALAR_TEST(FVectorArithmeticTest, NegationTest,
                                       negation, -, neg)

#define VECTOR_BINARY_OP_INPLACE_SCALAR_TEST(Suite, TestName, opName, op,      \
                                             expected)                         \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto &v = vals_.a[i];                                                    \
      auto &a = v.opName(vals_.scalar);                                        \
      auto e = vals_.au.expected.vec(d);                                       \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Eq(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      v = vals_.b[i];                                                          \
      a = v.opName(vals_.scalar);                                              \
      e = vals_.bu.expected.vec(d);                                            \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Eq(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

#define VECTOR_BINARY_OP_INPLACE_SCALAR_TESTS(Suite)                           \
  VECTOR_BINARY_OP_INPLACE_SCALAR_TEST(Suite, AddScalar, add, +, sum)          \
  VECTOR_BINARY_OP_INPLACE_SCALAR_TEST(Suite, SubtractScalar, subtract, -,     \
                                       dif)                                    \
  VECTOR_BINARY_OP_INPLACE_SCALAR_TEST(Suite, MultiplyScalar, multiply, *,     \
                                       prd)                                    \
  VECTOR_BINARY_OP_INPLACE_SCALAR_TEST(Suite, DivideScalar, divide, /, quo)

VECTOR_BINARY_OP_INPLACE_SCALAR_TESTS(IVectorArithmeticTest)
VECTOR_BINARY_OP_INPLACE_SCALAR_TESTS(FVectorArithmeticTest)
VECTOR_BINARY_OP_INPLACE_SCALAR_TESTS(DVectorArithmeticTest)

#define VECTOR_BINARY_OP_INPLACE_VECTOR_TEST(Suite, TestName, opName, op,      \
                                             expected)                         \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto v = vals_.a[i];                                                     \
      auto w = vals_.b[i];                                                     \
      auto a_or = v.opName(w);                                                 \
      auto e = vals_.ab.expected.vec(d);                                       \
      EXPECT_TRUE(a_or.ok());                                                  \
      auto a = a_or.ValueOrDie();                                              \
      EXPECT_EQ(v.dimension(), e.dimension());                                 \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      /* EXPECT_THAT(&v, testing::Eq(&e)); */                                  \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

#define VECTOR_BINARY_OP_INPLACE_VECTOR_TESTS(Suite)                           \
  VECTOR_BINARY_OP_INPLACE_VECTOR_TEST(Suite, AddVector, add, +, sum)          \
  VECTOR_BINARY_OP_INPLACE_VECTOR_TEST(Suite, SubtractVector, subtract, -,     \
                                       dif)                                    \
  VECTOR_BINARY_OP_INPLACE_VECTOR_TEST(Suite, MultiplyVector, multiply, *,     \
                                       prd)                                    \
  VECTOR_BINARY_OP_INPLACE_VECTOR_TEST(Suite, DivideVector, divide, /, quo)

VECTOR_BINARY_OP_INPLACE_VECTOR_TESTS(IVectorArithmeticTest)
VECTOR_BINARY_OP_INPLACE_VECTOR_TESTS(FVectorArithmeticTest)
VECTOR_BINARY_OP_INPLACE_VECTOR_TESTS(DVectorArithmeticTest)

#define VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TEST(Suite, TestName, opName, op,   \
                                                expected)                      \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto v = vals_.a[i];                                                     \
      auto a = v.opName(vals_.scalar);                                         \
      auto e = vals_.au.expected.vec(d);                                       \
      EXPECT_EQ(v.dimension(), e.dimension());                                 \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Ne(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

#define VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TESTS(Suite)                        \
  VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TEST(Suite, SumScalar, sum, +, sum)       \
  VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TEST(Suite, DifferenceScalar, difference, \
                                          -, dif)                              \
  VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TEST(Suite, ProductScalar, product, *,    \
                                          prd)                                 \
  VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TEST(Suite, QuotientScalar, quotient, /,  \
                                          quo)

VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TESTS(IVectorArithmeticTest)
VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TESTS(FVectorArithmeticTest)
VECTOR_BINARY_OP_OUTOFPLACE_SCALAR_TESTS(DVectorArithmeticTest)

#define VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TEST(Suite, TestName, opName, op,   \
                                                expected)                      \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto v = vals_.a[i];                                                     \
      auto w = vals_.b[i];                                                     \
      auto a_or = v.opName(w);                                                 \
      auto e = vals_.ab.expected.vec(d);                                       \
      EXPECT_TRUE(a_or.ok());                                                  \
      auto a = a_or.ValueOrDie();                                              \
      EXPECT_EQ(v.dimension(), e.dimension());                                 \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Ne(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

#define VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TESTS(Suite)                        \
  VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TEST(Suite, SumVector, add, +, sum)       \
  VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TEST(Suite, DifferenceFVector, subtract,  \
                                          -, dif)                              \
  VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TEST(Suite, ProductFVector, multiply, *,  \
                                          prd)                                 \
  VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TEST(Suite, QuotientFVector, divide, /,   \
                                          quo)

VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TESTS(IVectorArithmeticTest)
VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TESTS(FVectorArithmeticTest)
VECTOR_BINARY_OP_OUTOFPLACE_VECTOR_TESTS(DVectorArithmeticTest)

TEST_P(FVectorArithmeticTest, Dot) {
  int i = 0;
  for (int d = 2; d <= 4; d++) {
    auto v = vals_.a[i];
    auto w = vals_.b[i];
    auto a_or = v.dot(w);
    auto e = vals_.ab.d[i];
    EXPECT_TRUE(a_or.ok());
    auto a = a_or.ValueOrDie();
    EXPECT_THAT(a, testing::FloatEq(e));
    i++;
  }
}

TEST_P(FVectorArithmeticTest, Length) {
  int i = 0;
  for (int d = 2; d <= 4; d++) {
    auto v = vals_.a[i];
    auto a = v.length();
    auto e = vals_.as.l[i];
    EXPECT_THAT(a, testing::FloatEq(e));

    v = vals_.b[i];
    a = v.length();
    e = vals_.bs.l[i];
    EXPECT_THAT(a, testing::FloatEq(e));
    i++;
  }
}

INSTANTIATE_TEST_SUITE_P(
    Case1, IVectorArithmeticTest,
    testing::Values(VectorParamsTypeArith<IVector, int, float>(
        // a
        Quadruplet<IVector, int, float>(2, 3, 5, 8),
        // b
        Quadruplet<IVector, int, float>(1, 2, 3, 4),
        // s
        2)));
INSTANTIATE_TEST_SUITE_P(Case1, FVectorArithmeticTest,
                         testing::Values(VectorParamsTypeArith<>(
                             // a
                             Quadruplet<>(2, 3, 5, 8),
                             // b
                             Quadruplet<>(1, 2, 3, 4),
                             // s
                             2.5f)));
INSTANTIATE_TEST_SUITE_P(
    Case1, DVectorArithmeticTest,
    testing::Values(VectorParamsTypeArith<DVector, double, double>(
        // a
        Quadruplet<DVector, double, double>(2, 3, 5, 8),
        // b
        Quadruplet<DVector, double, double>(1, 2, 3, 4),
        // s
        2.5)));

INSTANTIATE_TEST_SUITE_P(
    Case2, IVectorArithmeticTest,
    testing::Values(VectorParamsTypeArith<IVector, int, float>(
        // a
        Quadruplet<IVector, int, float>(3, 2, -1, 0),
        // b
        Quadruplet<IVector, int, float>(-11, 6, 1, -9),
        // s
        -7)));
INSTANTIATE_TEST_SUITE_P(Case2, FVectorArithmeticTest,
                         testing::Values(VectorParamsTypeArith<>(
                             // a
                             Quadruplet<>(3.14, 2.97, -1.63, 0.52),
                             // b
                             Quadruplet<>(-11.2, 6.75, 1.69, -9.73),
                             // s
                             -7.31)));
INSTANTIATE_TEST_SUITE_P(
    Case2, DVectorArithmeticTest,
    testing::Values(VectorParamsTypeArith<DVector, double, double>(
        // a
        Quadruplet<DVector, double, double>(3.14, 2.97, -1.63, 0.52),
        // b
        Quadruplet<DVector, double, double>(-11.2, 6.75, 1.69, -9.73),
        // s
        -7.31)));

INSTANTIATE_TEST_SUITE_P(
    Case3, IVectorArithmeticTest,
    testing::Values(VectorParamsTypeArith<IVector, int, float>(
        // a
        Quadruplet<IVector, int, float>(0, 0, 0, 0),
        // b
        Quadruplet<IVector, int, float>(1, 1, 1, 1),
        // s
        -7)));
INSTANTIATE_TEST_SUITE_P(Case3, FVectorArithmeticTest,
                         testing::Values(VectorParamsTypeArith<>(
                             // a
                             Quadruplet<>(0, 0, 0, 0),
                             // b
                             Quadruplet<>(1, 1, 1, 1),
                             // s
                             -7.31)));
INSTANTIATE_TEST_SUITE_P(
    Case3, DVectorArithmeticTest,
    testing::Values(VectorParamsTypeArith<DVector, double, double>(
        // a
        Quadruplet<DVector, double, double>(0, 0, 0, 0),
        // b
        Quadruplet<DVector, double, double>(1, 1, 1, 1),
        // s
        -7.31)));

template <typename VectorType = FVector, typename FloatVectorType = FVector,
          typename DataType = float, typename FloatType = float>
struct QuadrupletUnaryTrig {
  Quadruplet<VectorType, DataType, FloatType> cos_i, sin_i, tan_i, acos_i,
      asin_i, atan_i, cosh_i, sinh_i, tanh_i, acosh_i, asinh_i, atanh_i;
  Quadruplet<FloatVectorType, FloatType, FloatType> cos, sin, tan, acos, asin,
      atan, cosh, sinh, tanh, acosh, asinh, atanh;

  explicit QuadrupletUnaryTrig(
      const Quadruplet<VectorType, DataType, FloatType> &v)
      : cos_i(::cos(v.x), ::cos(v.y), ::cos(v.z), ::cos(v.w)),
        sin_i(::sin(v.x), ::sin(v.y), ::sin(v.z), ::sin(v.w)),
        tan_i(::tan(v.x), ::tan(v.y), ::tan(v.z), ::tan(v.w)),
        acos_i(::acos(v.x), ::acos(v.y), ::acos(v.z), ::acos(v.w)),
        asin_i(::asin(v.x), ::asin(v.y), ::asin(v.z), ::asin(v.w)),
        atan_i(::atan(v.x), ::atan(v.y), ::atan(v.z), ::atan(v.w)),
        cosh_i(::cosh(v.x), ::cosh(v.y), ::cosh(v.z), ::cosh(v.w)),
        sinh_i(::sinh(v.x), ::sinh(v.y), ::sinh(v.z), ::sinh(v.w)),
        tanh_i(::tanh(v.x), ::tanh(v.y), ::tanh(v.z), ::tanh(v.w)),
        acosh_i(::acosh(v.x), ::acosh(v.y), ::acosh(v.z), ::acosh(v.w)),
        asinh_i(::asinh(v.x), ::asinh(v.y), ::asinh(v.z), ::asinh(v.w)),
        atanh_i(::atanh(v.x), ::atanh(v.y), ::atanh(v.z), ::atanh(v.w)),
        cos(::cos(v.x), ::cos(v.y), ::cos(v.z), ::cos(v.w)),
        sin(::sin(v.x), ::sin(v.y), ::sin(v.z), ::sin(v.w)),
        tan(::tan(v.x), ::tan(v.y), ::tan(v.z), ::tan(v.w)),
        acos(::acos(v.x), ::acos(v.y), ::acos(v.z), ::acos(v.w)),
        asin(::asin(v.x), ::asin(v.y), ::asin(v.z), ::asin(v.w)),
        atan(::atan(v.x), ::atan(v.y), ::atan(v.z), ::atan(v.w)),
        cosh(::cosh(v.x), ::cosh(v.y), ::cosh(v.z), ::cosh(v.w)),
        sinh(::sinh(v.x), ::sinh(v.y), ::sinh(v.z), ::sinh(v.w)),
        tanh(::tanh(v.x), ::tanh(v.y), ::tanh(v.z), ::tanh(v.w)),
        acosh(::acosh(v.x), ::acosh(v.y), ::acosh(v.z), ::acosh(v.w)),
        asinh(::asinh(v.x), ::asinh(v.y), ::asinh(v.z), ::asinh(v.w)),
        atanh(::atanh(v.x), ::atanh(v.y), ::atanh(v.z), ::atanh(v.w)) {}
  QuadrupletUnaryTrig(const QuadrupletUnaryTrig &o) = default;
};

template <typename VectorType = FVector, typename FloatVectorType = FVector,
          typename DataType = float, typename FloatType = float>
struct VectorParamsTypeTrig {
  const Quadruplet<VectorType, DataType, FloatType> vs;
  const QuadrupletUnaryTrig<VectorType, FloatVectorType, DataType, FloatType> u;
  std::vector<VectorType> v;
  explicit VectorParamsTypeTrig(
      const Quadruplet<VectorType, DataType, FloatType> &_v)
      : vs(_v), u(_v) {
    v.emplace_back(vs.x, vs.y);
    v.emplace_back(vs.x, vs.y, vs.z);
    v.emplace_back(vs.x, vs.y, vs.z, vs.w);
  }
};

class IVectorTrigTest
    : public testing::TestWithParam<
          VectorParamsTypeTrig<IVector, FVector, int, float>> {
protected:
  VectorParamsTypeTrig<IVector, FVector, int, float> vals_;
  IVectorTrigTest() : vals_(GetParam()) {}
};

class FVectorTrigTest : public testing::TestWithParam<VectorParamsTypeTrig<>> {
protected:
  VectorParamsTypeTrig<> vals_;
  FVectorTrigTest() : vals_(GetParam()) {}
};

class DVectorTrigTest
    : public testing::TestWithParam<
          VectorParamsTypeTrig<DVector, DVector, double, double>> {
protected:
  VectorParamsTypeTrig<DVector, DVector, double, double> vals_;
  DVectorTrigTest() : vals_(GetParam()) {}
};

#define VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, TestName, fn,          \
                                                 expected)                     \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto &v = vals_.v[i];                                                    \
      auto &a = v.fn();                                                        \
      auto e = vals_.u.expected.vec(d);                                        \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Eq(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

#define VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TESTS(Suite)                       \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, CosInplaceTest, cos, cos_i)  \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, SinInplaceTest, sin, sin_i)  \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, TanInplaceTest, tan, tan_i)  \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, AcosInplaceTest, acos,       \
                                           acos_i)                             \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, AsinInplaceTest, asin,       \
                                           asin_i)                             \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, AtanInplaceTest, atan,       \
                                           atan_i)                             \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, CoshInplaceTest, cosh,       \
                                           cosh_i)                             \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, SinhInplaceTest, sinh,       \
                                           sinh_i)                             \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, TanhInplaceTest, tanh,       \
                                           tanh_i)                             \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, AcoshInplaceTest, acosh,     \
                                           acosh_i)                            \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, AsinhInplaceTest, asinh,     \
                                           asinh_i)                            \
  VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TEST(Suite, AtanhInplaceTest, atanh,     \
                                           atanh_i)

VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TESTS(IVectorTrigTest)
VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TESTS(FVectorTrigTest)
VECTOR_UNARY_TRIG_FN_INPLACE_SCALAR_TESTS(DVectorTrigTest)

#define VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, TestName, fn,       \
                                                    expected, vtype, fvtype)   \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      vtype v = vals_.v[i];                                                    \
      fvtype a = v.fn();                                                       \
      fvtype e = vals_.u.expected.vec(d);                                      \
      EXPECT_EQ(v.dimension(), e.dimension());                                 \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

#define VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TESTS(Suite, vtype, fvtype)     \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, CosOutofplaceTest,        \
                                              cos_of, cos, vtype, fvtype)      \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, SinOutofplaceTest,        \
                                              sin_of, sin, vtype, fvtype)      \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, TanOutofplaceTest,        \
                                              tan_of, tan, vtype, fvtype)      \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, AcosOutofplaceTest,       \
                                              acos_of, acos, vtype, fvtype)    \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, AsinOutofplaceTest,       \
                                              asin_of, asin, vtype, fvtype)    \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, AtanOutofplaceTest,       \
                                              atan_of, atan, vtype, fvtype)    \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, CoshOutofplaceTest,       \
                                              cosh_of, cosh, vtype, fvtype)    \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, SinhOutofplaceTest,       \
                                              sinh_of, sinh, vtype, fvtype)    \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, TanhOutofplaceTest,       \
                                              tanh_of, tanh, vtype, fvtype)    \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, AcoshOutofplaceTest,      \
                                              acosh_of, acosh, vtype, fvtype)  \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, AsinhOutofplaceTest,      \
                                              asinh_of, asinh, vtype, fvtype)  \
  VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TEST(Suite, AtanhOutofplaceTest,      \
                                              atanh_of, atanh, vtype, fvtype)

VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TESTS(IVectorTrigTest, IVector, FVector)
VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TESTS(FVectorTrigTest, FVector, FVector)
VECTOR_UNARY_TRIG_FN_OUTOFPLACE_SCALAR_TESTS(DVectorTrigTest, DVector, DVector)

/*
INSTANTIATE_TEST_SUITE_P(
Case1, IVectorTrigTest,
testing::Values(VectorParamsTypeTrig<IVector, FVector, int, float>(
    Quadruplet<IVector, int, float>(1, 1, 1, 1))));
INSTANTIATE_TEST_SUITE_P(
Case1, FVectorTrigTest,
testing::Values(VectorParamsTypeTrig<>(Quadruplet<>(1, 1, 1, 1))));
INSTANTIATE_TEST_SUITE_P(
Case1, DVectorTrigTest,
testing::Values(VectorParamsTypeTrig<DVector, DVector, double, double>(
    Quadruplet<DVector, double, double>(1, 1, 1, 1))));
*/

#define VECTOR_UNARY_FC_FN_INPLACE_SCALAR_TEST(Suite, TestName, fn, expected)  \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto &v = vals_.v[i];                                                    \
      auto &a = v.fn();                                                        \
      auto e = vals_.u.expected.vec(d);                                        \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Eq(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

#define VECTOR_UNARY_FC_FN_INPLACE_SCALAR_TESTS(Suite)                         \
  VECTOR_UNARY_FC_FN_INPLACE_SCALAR_TEST(Suite, FloorInplaceTest, floor,       \
                                         floor_i)                              \
  VECTOR_UNARY_FC_FN_INPLACE_SCALAR_TEST(Suite, CeilInplaceTest, ceil, ceil_i)

#define VECTOR_UNARY_FN_OUTOFPLACE_SCALAR_TEST(Suite, TestName, fn, expected)  \
  TEST_P(Suite, TestName) {                                                    \
    int i = 0;                                                                 \
    for (int d = 2; d <= 4; d++) {                                             \
      auto v = vals_.v[i];                                                     \
      auto a = v.fn();                                                         \
      auto e = vals_.u.expected.vec(d);                                        \
      EXPECT_EQ(v.dimension(), e.dimension());                                 \
      EXPECT_EQ(a.dimension(), e.dimension());                                 \
      EXPECT_THAT(&v, testing::Ne(&a));                                        \
      EXPECT_THAT(a, VectorEq(e));                                             \
      i++;                                                                     \
    }                                                                          \
  }

#define VECTOR_UNARY_FN_OUTOFPLACE_SCALAR_TESTS(Suite)                         \
  VECTOR_UNARY_FN_OUTOFPLACE_SCALAR_TEST(Suite, FloorOutofplaceTest, floor,    \
                                         floor)                                \
  VECTOR_UNARY_FN_OUTOFPLACE_SCALAR_TEST(Suite, CeilOutofplaceTest, ceil, ceil)

} // namespace

} // namespace vector
} // namespace math
