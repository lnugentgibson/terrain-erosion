#include "math/vector/vector.h"

#include "gtest/gtest.h"

using math::vector::Vector;

TEST(VectorTest, AddVector) {
  Vector a(2, 3, 5), b(1, 2, 3);
  auto c = a.add(b);
  EXPECT_TRUE(c.HasValue());
  EXPECT_EQ((*c)[0], 3);
  EXPECT_EQ((*c)[1], 5);
  EXPECT_EQ((*c)[2], 8);
}
 
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}