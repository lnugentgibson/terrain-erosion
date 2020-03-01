#include "experimental/users/nugentgibson/graph/default_simple_graph.h"

#include <algorithm>

#include "third_party/absl/status/status.h"

Node DefaultSimpleGraph::AddNode() {
  size_t id;
  Node node;
  if (free_node_indices_.empty()) {
    id = Order();
    node = Node(id);
    nodes_.push_back({node, {}});
  } else {
    auto begin = free_node_indices_.begin();
    id = *begin;
    node = Node(id);
    free_node_indices_.erase(begin);
    nodes_[id] = {node, {}};
  }
  return node;
}

absl::Status DefaultSimpleGraph::RemoveNode(Node id) {
  if (!HasNode(id)) {
    return absl::Status(absl::StatusCode::kNotFound, "");
  }
  auto [node, neighbors] = nodes_[id.Id()];
  for (auto neighbor : neighbors) {
    auto [incident, adjacent] = neighbor;
    RemoveEdge(incident).IgnoreError();
  }
  free_node_indices_.insert(id.Id());
  return absl::OkStatus();
}

util::StatusOr<Edge> DefaultSimpleGraph::AddEdge(Node a, Node b) {
  if (a == b) {
    return absl::Status(absl::StatusCode::kInvalidArgument,
                        "Cannot connect node to itself");
  }
  auto adj = Adjacent(a, b);
  if (!adj.ok()) {
    return adj.status();
  } else if (adj.ValueOrDie()) {
    return absl::Status(absl::StatusCode::kAlreadyExists,
                        "Nodes are already adjacent");
  }
  auto [a_node, a_neighbors] = nodes_[a.Id()];
  auto [b_node, b_neighbors] = nodes_[b.Id()];
  size_t id;
  Edge edge;
  if (free_edge_indices_.empty()) {
    id = Size();
    edge = Edge(id);
    edges_.push_back({edge, {a_node, b_node}});
  } else {
    auto begin = free_edge_indices_.begin();
    id = *begin;
    edge = Edge(id);
    free_edge_indices_.erase(begin);
    edges_[id] = {edge, {a_node, b_node}};
  }
  return edge;
}

absl::Status DefaultSimpleGraph::RemoveEdge(Edge id) {
  if (!HasEdge(id)) {
    return absl::Status(absl::StatusCode::kNotFound, "");
  }
  auto [edge, endpoints] = edges_[id.Id()];
  auto [end_a, end_b] = endpoints;
  auto [a_node, a_neighbors] = nodes_[end_a.Id()];
  auto [b_node, b_neighbors] = nodes_[end_b.Id()];
  auto a_b = a_neighbors.find(id);
  if (a_b == a_neighbors.end()) {
    return absl::Status(absl::StatusCode::kInternal,
                        "edge not found in Node a's neighborhood");
  }
  if (a_b->second != b_node) {
    return absl::Status(absl::StatusCode::kInternal,
                        "edge does not point to b from a");
  }
  a_neighbors.erase(a_b);
  auto b_a = b_neighbors.find(id);
  if (b_a == b_neighbors.end()) {
    return absl::Status(absl::StatusCode::kInternal,
                        "edge not found in Node b's neighborhood");
  }
  if (b_a->second != a_node) {
    return absl::Status(absl::StatusCode::kInternal,
                        "edge does not point to a from b");
  }
  b_neighbors.erase(b_a);
  free_edge_indices_.insert(id.Id());
  return absl::OkStatus();
}

util::StatusOr<bool> DefaultSimpleGraph::Adjacent(Node a, Node b) {
  if (a == b) {
    return absl::Status(absl::StatusCode::kInvalidArgument,
                        "Node cannot be connected to itself");
    // return false;
  }
  if (!HasNode(a)) {
    return absl::Status(absl::StatusCode::kNotFound, "Node a not found");
  }
  if (!HasNode(b)) {
    return absl::Status(absl::StatusCode::kNotFound, "Node b not found");
  }
  auto [a_node, a_neighbors] = nodes_[a.Id()];
  auto it = std::find_if(a_neighbors.begin(), a_neighbors.end(),
                         [b](const std::pair<Edge, Node> &neighbor) {
                           return neighbor.second == b;
                         });
  return it != a_neighbors.end();
}

util::StatusOr<bool> DefaultSimpleGraph::Incident(Node node, Edge edge) {
  if (!HasNode(node)) {
    return absl::Status(absl::StatusCode::kNotFound, "Node not found");
  }
  if (!HasEdge(edge)) {
    return absl::Status(absl::StatusCode::kNotFound, "Edge not found");
  }
  auto [node_, neighbors] = nodes_[node.Id()];
  auto it = neighbors.find(edge);
  return it != neighbors.end();
}

util::StatusOr<Edge> DefaultSimpleGraph::GetEdge(Node a, Node b) {
  if (a == b) {
    return absl::Status(absl::StatusCode::kInvalidArgument,
                        "Node cannot be connected to itself");
  }
  if (!HasNode(a)) {
    return absl::Status(absl::StatusCode::kNotFound, "Node a not found");
  }
  if (!HasNode(b)) {
    return absl::Status(absl::StatusCode::kNotFound, "Node b not found");
  }
  auto [a_node, a_neighbors] = nodes_[a.Id()];
  auto it = std::find_if(a_neighbors.begin(), a_neighbors.end(),
                         [b](const std::pair<Edge, Node> &neighbor) {
                           return neighbor.second == b;
                         });
  if (it != a_neighbors.end()) {
    return absl::Status(absl::StatusCode::kNotFound, "Nodes are adjacent");
  }
  return it->first;
}

absl::Status DefaultSimpleGraph::GetNeighbors(
    Node id, std::vector<std::pair<Edge, Node>> *neighbors) {
  if (!HasNode(id)) {
    return absl::Status(absl::StatusCode::kNotFound, "Node not found");
  }
  auto [node_, neighborhood] = nodes_[id.Id()];
  for (auto neighbor : neighborhood) {
    neighbors->push_back(neighbor);
  }
  return absl::OkStatus();
}

util::StatusOr<std::pair<Node, Node>>
DefaultSimpleGraph::GetEndpoints(Edge id) {
  if (!HasEdge(id)) {
    return absl::Status(absl::StatusCode::kNotFound, "");
  }
  auto [edge, endpoints] = edges_[id.Id()];
  return endpoints;
}
