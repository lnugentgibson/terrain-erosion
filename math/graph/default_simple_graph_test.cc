#include "experimental/users/nugentgibson/graph/default_simple_graph.h"

#include <memory>

#include "testing/base/public/gmock.h"
#include "testing/base/public/gunit.h"

namespace {

class DefaultSimpleGraphTest : public testing::Test {
protected:
  DefaultSimpleGraphTest() : graph_(new DefaultSimpleGraph()) {}
  std::unique_ptr<SimpleGraph> graph_;
};

TEST_F(DefaultSimpleGraphTest, EmptyGraphSizes) {
  EXPECT_EQ(graph_->Order(), 0);
  EXPECT_EQ(graph_->Size(), 0);
  EXPECT_TRUE(graph_->IsEmpty());
}

TEST_F(DefaultSimpleGraphTest, AddSingleNode) {
  EXPECT_EQ(graph_->AddNode(), 0);
  EXPECT_TRUE(graph_->HasNode(Node(0)));
  EXPECT_EQ(graph_->Order(), 1);
  EXPECT_EQ(graph_->Size(), 0);
  EXPECT_FALSE(graph_->IsEmpty());
}

TEST_F(DefaultSimpleGraphTest, HasNonexistantNode) {
  EXPECT_FALSE(graph_->HasNode(Node(0)));
}

TEST_F(DefaultSimpleGraphTest, AddMultipleNodes) {
  EXPECT_EQ(graph_->AddNode(), 0);
  EXPECT_EQ(graph_->AddNode(), 1);
  EXPECT_TRUE(graph_->HasNode(Node(0)));
  EXPECT_TRUE(graph_->HasNode(Node(1)));
  EXPECT_EQ(graph_->Order(), 2);
  EXPECT_EQ(graph_->Size(), 0);
  EXPECT_FALSE(graph_->IsEmpty());
  EXPECT_EQ(graph_->AddNode(), 2);
  EXPECT_TRUE(graph_->HasNode(Node(0)));
  EXPECT_TRUE(graph_->HasNode(Node(1)));
  EXPECT_TRUE(graph_->HasNode(Node(2)));
  EXPECT_EQ(graph_->Order(), 3);
  EXPECT_EQ(graph_->Size(), 0);
  EXPECT_FALSE(graph_->IsEmpty());
}

TEST_F(DefaultSimpleGraphTest, RemoveSingleNode) {
  EXPECT_EQ(graph_->AddNode(), 0);
  EXPECT_EQ(graph_->AddNode(), 1);
  EXPECT_EQ(graph_->AddNode(), 2);
  EXPECT_OK(graph_->RemoveNode(Node(1)));
  EXPECT_TRUE(graph_->HasNode(Node(0)));
  EXPECT_FALSE(graph_->HasNode(Node(1)));
  EXPECT_TRUE(graph_->HasNode(Node(2)));
  EXPECT_EQ(graph_->Order(), 2);
  EXPECT_EQ(graph_->Size(), 0);
  EXPECT_FALSE(graph_->IsEmpty());
}

TEST_F(DefaultSimpleGraphTest, ReusesNodeIndices) {
  EXPECT_EQ(graph_->AddNode(), 0);
  EXPECT_EQ(graph_->AddNode(), 1);
  EXPECT_EQ(graph_->AddNode(), 2);
  EXPECT_OK(graph_->RemoveNode(Node(1)));
  EXPECT_EQ(graph_->AddNode(), 1);
  EXPECT_TRUE(graph_->HasNode(Node(0)));
  EXPECT_TRUE(graph_->HasNode(Node(1)));
  EXPECT_TRUE(graph_->HasNode(Node(2)));
  EXPECT_EQ(graph_->Order(), 3);
  EXPECT_EQ(graph_->Size(), 0);
  EXPECT_FALSE(graph_->IsEmpty());
}

} // namespace
