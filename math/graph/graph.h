#ifndef EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_GRAPH_H_
#define EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_GRAPH_H_

#include <utility>
#include <vector>

#include "third_party/absl/status/status.h"
#include "util/task/statusor.h"

class Node {
public:
  explicit Node() : id_(SIZE_MAX) {}
  explicit Node(size_t id) : id_(id) {}
  Node &operator=(const Node &node) {
    id_ = node.id_;
    return *this;
  }
  Node &operator=(size_t id) {
    id_ = id;
    return *this;
  }

  explicit operator size_t() { return id_; }
  bool operator<(const Node &id) const { return id_ < id.id_; }
  bool operator<(size_t id) const { return id_ < id; }
  bool operator<=(const Node &id) const { return id_ <= id.id_; }
  bool operator<=(size_t id) const { return id_ <= id; }
  bool operator==(const Node &id) const { return id_ == id.id_; }
  bool operator==(size_t id) const { return id_ == id; }
  bool operator!=(const Node &id) const { return id_ != id.id_; }
  bool operator!=(size_t id) const { return id_ != id; }
  bool operator>=(const Node &id) const { return id_ >= id.id_; }
  bool operator>=(size_t id) const { return id_ >= id; }
  bool operator>(const Node &id) const { return id_ > id.id_; }
  bool operator>(size_t id) const { return id_ > id; }

  inline size_t Id() { return id_; }

private:
  size_t id_;
};

class Edge {
public:
  explicit Edge() : id_(SIZE_MAX) {}
  explicit Edge(size_t id) : id_(id) {}
  Edge &operator=(const Edge &node) {
    id_ = node.id_;
    return *this;
  }
  Edge &operator=(size_t id) {
    id_ = id;
    return *this;
  }

  explicit operator size_t() const { return id_; }
  bool operator<(const Edge &id) const { return id_ < id.id_; }
  bool operator<(size_t id) const { return id_ < id; }
  bool operator<=(const Edge &id) const { return id_ <= id.id_; }
  bool operator<=(size_t id) const { return id_ <= id; }
  bool operator==(const Edge &id) const { return id_ == id.id_; }
  bool operator==(size_t id) const { return id_ == id; }
  bool operator!=(const Edge &id) const { return id_ != id.id_; }
  bool operator!=(size_t id) const { return id_ != id; }
  bool operator>=(const Edge &id) const { return id_ >= id.id_; }
  bool operator>=(size_t id) const { return id_ >= id; }
  bool operator>(const Edge &id) const { return id_ > id.id_; }
  bool operator>(size_t id) const { return id_ > id; }

  inline size_t Id() { return id_; }

private:
  size_t id_;
};

class Graph {
public:
  virtual ~Graph() {}

  // The number of nodes.
  virtual size_t Order() = 0;

  // The number of edges.
  virtual size_t Size() = 0;

  virtual bool IsEmpty() = 0;

  virtual Node AddNode() = 0;

  virtual bool HasNode(Node id) = 0;

  virtual absl::Status RemoveNode(Node id) = 0;

  virtual util::StatusOr<Edge> AddEdge(Node a, Node b) = 0;

  virtual bool HasEdge(Edge id) = 0;

  virtual absl::Status RemoveEdge(Edge id) = 0;

  virtual util::StatusOr<bool> Adjacent(Node a, Node b) = 0;

  virtual util::StatusOr<bool> Incident(Node node, Edge edge) = 0;

  virtual absl::Status GetEdges(Node a, Node b,
                                std::vector<Edge> *neighbors) = 0;

  virtual absl::Status
  GetNeighbors(Node id, std::vector<std::pair<Edge, Node>> *neighbors) = 0;

  virtual util::StatusOr<std::pair<Node, Node>> GetEndpoints(Edge id) = 0;

  // The length of the shortest path between the specified nodes.
  //
  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int> Distance(Node a, Node b) = 0;

  // The greatest distance between the specified node and any other node.
  //
  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int> Eccentricity(Node node) = 0;

  // The minimum eccentricity of any node.
  //
  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int> Radius() = 0;

  // The maximum eccentricity of any node.
  //
  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int> Diameter() = 0;

  // The length of the shortest cycle.
  //
  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int> Girth() = 0;

  // The length of the longest cycle.
  //
  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int> Circumference() = 0;
};

class SimpleGraph : public Graph {
public:
  virtual util::StatusOr<Edge> GetEdge(Node a, Node b) = 0;

  absl::Status GetEdges(Node a, Node b, std::vector<Edge> *neighbors) override {
    auto edge = GetEdge(a, b);
    if (edge.ok()) {
      neighbors->push_back(edge.ValueOrDie());
      return absl::OkStatus();
    } else {
      return edge.status();
    }
  }
};

#endif // EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_GRAPH_H_
