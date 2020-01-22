#include "gtest/gtest.h"

TEST(HelloTest, GetGreet) {
  EXPECT_EQ("Hello Bazel", "Hello Bazel");
}
 
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}