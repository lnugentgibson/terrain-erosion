#ifndef EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_DEFAULT_SIMPLE_GRAPH_H_
#define EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_DEFAULT_SIMPLE_GRAPH_H_

#include <map>
#include <set>

#include "experimental/users/nugentgibson/graph/graph.h"
#include "third_party/absl/status/status.h"

class DefaultSimpleGraph : public SimpleGraph {
 public:
  // The number of nodes.
  size_t Order() override { return nodes_.size() - free_node_indices_.size(); }

  // The number of edges.
  size_t Size() override { return edges_.size() - free_edge_indices_.size(); }

  bool IsEmpty() override { return nodes_.empty(); }

  Node AddNode() override;

  bool HasNode(Node id) override {
    if (id.Id() >= nodes_.size()) {
      return false;
    }
    auto it = free_node_indices_.find(id.Id());
    return it == free_node_indices_.end();
  }

  absl::Status RemoveNode(Node id) override;

  util::StatusOr<Edge> AddEdge(Node a, Node b) override;

  bool HasEdge(Edge id) override {
    if (id.Id() >= edges_.size()) {
      return false;
    }
    auto it = free_edge_indices_.find(id.Id());
    return it == free_edge_indices_.end();
  }

  absl::Status RemoveEdge(Edge id) override;

  // Whether nodes a and b are connect by an edge.
  // Returns kNotFound if either of the nodes do not exist.
  util::StatusOr<bool> Adjacent(Node a, Node b) override;

  // Whether the node is an endpoint of the edge.
  // Returns kNotFound if either the node or edge does not exist.
  util::StatusOr<bool> Incident(Node node, Edge edge) override;

  // The id of the edge connecting the two nodes.
  // Returns kNotFound if either of the nodes do not exist. or if the nodes are
  // not adjacent.
  util::StatusOr<Edge> GetEdge(Node a, Node b) override;

  // The edges and nodes connected to this node.
  absl::Status GetNeighbors(
      Node id, std::vector<std::pair<Edge, Node>>* neighbors) override;

  util::StatusOr<std::pair<Node, Node>> GetEndpoints(Edge id) override;

  // The length of the shortest path between the specified nodes.
  //
  // Not required. Will return kUnimplemented if not implemented.
  util::StatusOr<int> Distance(Node a, Node b) override {
    return absl::Status(absl::StatusCode::kUnimplemented, "");
  }

  // The greatest distance between the specified node and any other node.
  //
  // Not required. Will return kUnimplemented if not implemented.
  util::StatusOr<int> Eccentricity(Node node) override {
    return absl::Status(absl::StatusCode::kUnimplemented, "");
  }

  // The minimum eccentricity of any node.
  //
  // Not required. Will return kUnimplemented if not implemented.
  util::StatusOr<int> Radius() override {
    return absl::Status(absl::StatusCode::kUnimplemented, "");
  }

  // The maximum eccentricity of any node.
  //
  // Not required. Will return kUnimplemented if not implemented.
  util::StatusOr<int> Diameter() override {
    return absl::Status(absl::StatusCode::kUnimplemented, "");
  }

  // The length of the shortest cycle.
  //
  // Not required. Will return kUnimplemented if not implemented.
  util::StatusOr<int> Girth() override {
    return absl::Status(absl::StatusCode::kUnimplemented, "");
  }

  // The length of the longest cycle.
  //
  // Not required. Will return kUnimplemented if not implemented.
  util::StatusOr<int> Circumference() override {
    return absl::Status(absl::StatusCode::kUnimplemented, "");
  }

 private:
  std::vector<std::pair<Node, std::map<Edge, Node>>> nodes_;
  std::set<size_t> free_node_indices_;
  std::vector<std::pair<Edge, std::pair<Node, Node>>> edges_;
  std::set<size_t> free_edge_indices_;
};

#endif  // EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_DEFAULT_SIMPLE_GRAPH_H_
