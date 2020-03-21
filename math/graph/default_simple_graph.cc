#include "math/graph/default_simple_graph.h"

#include <algorithm>

#include "util/status.h"

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

util::Status DefaultSimpleGraph::RemoveNode(Node id) {
  if (!HasNode(id)) {
    return util::Status(util::StatusCode::NOT_FOUND, "");
  }
  auto neighbors = nodes_[id.Id()].second;
  for (auto neighbor : neighbors) {
    auto incident = neighbor.first;
    RemoveEdge(incident).IgnoreError();
  }
  free_node_indices_.insert(id.Id());
  return util::Status::OK();
}

util::StatusOr<Edge> DefaultSimpleGraph::AddEdge(Node a, Node b) {
  if (a == b) {
    return util::Status(util::StatusCode::INVALID_ARGUMENT,
                        "Cannot connect node to itself");
  }
  auto adj = Adjacent(a, b);
  if (!adj.ok()) {
    return adj.status();
  } else if (adj.ValueOrDie()) {
    return util::Status(util::StatusCode::ALREADY_EXISTS,
                        "Nodes are already adjacent");
  }
  auto a_node = nodes_[a.Id()].first;
  auto b_node = nodes_[b.Id()].first;
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

util::Status DefaultSimpleGraph::RemoveEdge(Edge id) {
  if (!HasEdge(id)) {
    return util::Status(util::StatusCode::NOT_FOUND, "");
  }
  auto endpoints = edges_[id.Id()].second;
  auto [end_a, end_b] = endpoints;
  auto [a_node, a_neighbors] = nodes_[end_a.Id()];
  auto [b_node, b_neighbors] = nodes_[end_b.Id()];
  auto a_b = a_neighbors.find(id);
  if (a_b == a_neighbors.end()) {
    return util::Status(util::StatusCode::INTERNAL,
                        "edge not found in Node a's neighborhood");
  }
  if (a_b->second != b_node) {
    return util::Status(util::StatusCode::INTERNAL,
                        "edge does not point to b from a");
  }
  a_neighbors.erase(a_b);
  auto b_a = b_neighbors.find(id);
  if (b_a == b_neighbors.end()) {
    return util::Status(util::StatusCode::INTERNAL,
                        "edge not found in Node b's neighborhood");
  }
  if (b_a->second != a_node) {
    return util::Status(util::StatusCode::INTERNAL,
                        "edge does not point to a from b");
  }
  b_neighbors.erase(b_a);
  free_edge_indices_.insert(id.Id());
  return util::Status::OK();
}

util::StatusOr<bool> DefaultSimpleGraph::Adjacent(Node a, Node b) {
  if (a == b) {
    return util::Status(util::StatusCode::INVALID_ARGUMENT,
                        "Node cannot be connected to itself");
    // return false;
  }
  if (!HasNode(a)) {
    return util::Status(util::StatusCode::NOT_FOUND, "Node a not found");
  }
  if (!HasNode(b)) {
    return util::Status(util::StatusCode::NOT_FOUND, "Node b not found");
  }
  auto a_neighbors = nodes_[a.Id()].second;
  auto it = std::find_if(a_neighbors.begin(), a_neighbors.end(),
                         [b](const std::pair<Edge, Node> &neighbor) {
                           return neighbor.second == b;
                         });
  return it != a_neighbors.end();
}

util::StatusOr<bool> DefaultSimpleGraph::Incident(Node node, Edge edge) {
  if (!HasNode(node)) {
    return util::Status(util::StatusCode::NOT_FOUND, "Node not found");
  }
  if (!HasEdge(edge)) {
    return util::Status(util::StatusCode::NOT_FOUND, "Edge not found");
  }
  auto neighbors = nodes_[node.Id()].second;
  auto it = neighbors.find(edge);
  return it != neighbors.end();
}

util::StatusOr<Edge> DefaultSimpleGraph::GetEdge(Node a, Node b) {
  if (a == b) {
    return util::Status(util::StatusCode::INVALID_ARGUMENT,
                        "Node cannot be connected to itself");
  }
  if (!HasNode(a)) {
    return util::Status(util::StatusCode::NOT_FOUND, "Node a not found");
  }
  if (!HasNode(b)) {
    return util::Status(util::StatusCode::NOT_FOUND, "Node b not found");
  }
  auto a_neighbors = nodes_[a.Id()].second;
  auto it = std::find_if(a_neighbors.begin(), a_neighbors.end(),
                         [b](const std::pair<Edge, Node> &neighbor) {
                           return neighbor.second == b;
                         });
  if (it != a_neighbors.end()) {
    return util::Status(util::StatusCode::NOT_FOUND, "Nodes are adjacent");
  }
  return it->first;
}

util::Status DefaultSimpleGraph::GetNeighbors(
    Node id, std::vector<std::pair<Edge, Node>> *neighbors) {
  if (!HasNode(id)) {
    return util::Status(util::StatusCode::NOT_FOUND, "Node not found");
  }
  auto neighborhood = nodes_[id.Id()].second;
  for (auto neighbor : neighborhood) {
    neighbors->push_back(neighbor);
  }
  return util::Status::OK();
}

util::StatusOr<std::pair<Node, Node>>
DefaultSimpleGraph::GetEndpoints(Edge id) {
  if (!HasEdge(id)) {
    return util::Status(util::StatusCode::NOT_FOUND, "");
  }
  auto endpoints = edges_[id.Id()].second;
  return endpoints;
}
