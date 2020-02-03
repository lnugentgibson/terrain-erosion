#include "container/spatial/grid.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include "gtest/gtest.h"

namespace container {
namespace spatial {

namespace {

TEST(GridTest, Constructor) {
    int sizes[2] {4, 5};
    Grid<std::string> grid(2, sizes, Vector(2, 0, 0), Vector(2, 4, 5), Vector(2, 1, 1));
  EXPECT_TRUE(true);
}

} // namespace

} // namespace spatial
} // namespace container

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}