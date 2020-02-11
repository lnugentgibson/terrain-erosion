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
  int sizes[2]{4, 5};
  Grid grid(2, sizes, Vector(0, 0), Vector(4, 5), Vector(1, 1));
  EXPECT_TRUE(true);
}

TEST(GridTest, Insert) {
  int sizes[2]{4, 5};
  Grid grid(2, sizes, Vector(0, 0), Vector(4, 5), Vector(1, 1));
  size_t id = grid.insert(Vector(2, 3));
  EXPECT_EQ(id, 0);
  EXPECT_TRUE(true);
}

//*
TEST(GridTest, InsertMultiple) {
  int sizes[2]{4, 5};
  Grid grid(2, sizes, Vector(0, 0), Vector(4, 5), Vector(1, 1));
  size_t id = grid.insert(Vector(2, 3));
  EXPECT_EQ(id, 0);
  id = grid.insert(Vector(3, 4));
  EXPECT_EQ(id, 1);
  EXPECT_TRUE(true);
}
//*/

} // namespace

} // namespace spatial
} // namespace container

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}