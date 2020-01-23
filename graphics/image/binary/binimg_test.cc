#include "gtest/gtest.h"

class NeighborhoodTest : public testing::Test {
 protected:
};

TEST_F(NeighborhoodTest, Test) {
  EXPECT_EQ("Hello Bazel", "Hello Bazel");
}

TEST(HelloTest, GetGreet) {
  EXPECT_EQ("Hello Bazel", "Hello Bazel");
}
 
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}