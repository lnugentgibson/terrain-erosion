#ifndef EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_GRAPH_H_
#define EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_GRAPH_H_

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

// #include "net/proto2/public/message.h"
#include "absl/strings/numbers.h"
#include "absl/strings/substitute.h"
#include "util/status.h"
#include "util/statusor.h"

class Node {
public:
  explicit Node() : id_(SIZE_MAX) {}
  explicit Node(size_t id) : id_(id) {}
  Node(const Node &id) : id_(id.id_) {}
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
  Edge(const Edge &id) : id_(id.id_) {}
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

  virtual util::Status RemoveNode(Node id) = 0;

  virtual util::StatusOr<Edge> AddEdge(Node a, Node b) = 0;

  virtual bool HasEdge(Edge id) = 0;

  virtual util::Status RemoveEdge(Edge id) = 0;

  virtual util::StatusOr<bool> Adjacent(Node a, Node b) = 0;

  virtual util::StatusOr<bool> Incident(Node node, Edge edge) = 0;

  virtual util::Status GetEdges(Node a, Node b,
                                std::vector<Edge> *neighbors) = 0;

  virtual util::Status
  GetNeighbors(Node id, std::vector<std::pair<Edge, Node>> *neighbors) = 0;

  virtual util::StatusOr<std::pair<Node, Node>> GetEndpoints(Edge id) = 0;
};

class SimpleGraph : public Graph {
public:
  virtual util::StatusOr<Edge> GetEdge(Node a, Node b) = 0;

  util::Status GetEdges(Node a, Node b, std::vector<Edge> *neighbors) override {
    auto edge = GetEdge(a, b);
    if (edge.ok()) {
      neighbors->push_back(edge.ValueOrDie());
      return util::Status::OK();
    } else {
      return edge.status();
    }
  }
};

class LabeledGraph {
public:
  virtual ~LabeledGraph() {}

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<std::string> GetNodeLabel(Node id) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<std::string> GetEdgeLabel(Edge id) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetNodeLabel(Node id, std::string label) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetEdgeLabel(Edge id, std::string label) = 0;
};

class Property {
public:
  enum DataType { INT32, INT64, FLOAT, DOUBLE, STRING, PROTO, POINTER };

  Property() : type_(INT32) {}

  explicit Property(DataType type) : type_(type) {
    if (type_ == STRING) {
      string_value_ = "";
    }
    // if (type_ == PROTO) {
    //  proto_value_ = nullptr;
    //}
    if (type_ == POINTER) {
      numeric_.pointer_value = nullptr;
    }
  }

  explicit Property(int32_t i) { ForceInt32Value(i); }
  explicit Property(int64_t l) { ForceInt64Value(l); }
  explicit Property(float f) { ForceFloatValue(f); }
  explicit Property(double d) { ForceDoubleValue(d); }
  explicit Property(std::string s) { ForceStringValue(s); }
  // explicit Property(proto2::Message *m) { ForceProtoValue(m); }

  DataType Type() { return type_; }

  int32_t Int32Value() {
    switch (type_) {
    case INT32:
      return numeric_.int32_t_value;
    case INT64:
      return numeric_.int64_t_value;
    case FLOAT:
      return numeric_.float_value;
    case DOUBLE:
      return numeric_.double_value;
    case STRING:
      int32_t out;
      if (!absl::SimpleAtoi(*string_value_, &out)) {
        return 0;
      }
      return out;
      break;
    default:
      return 0;
    }
  }
  int64_t Int64Value() {
    switch (type_) {
    case INT32:
      return numeric_.int32_t_value;
    case INT64:
      return numeric_.int64_t_value;
    case FLOAT:
      return numeric_.float_value;
    case DOUBLE:
      return numeric_.double_value;
    case STRING:
      int64_t out;
      if (!absl::SimpleAtoi(*string_value_, &out)) {
        return 0;
      }
      return out;
      break;
    default:
      return 0;
    }
  }
  float FloatValue() {
    switch (type_) {
    case INT32:
      return numeric_.int32_t_value;
    case INT64:
      return numeric_.int64_t_value;
    case FLOAT:
      return numeric_.float_value;
    case DOUBLE:
      return numeric_.double_value;
    case STRING:
      float out;
      if (!absl::SimpleAtof(*string_value_, &out)) {
        return 0;
      }
      return out;
      break;
    default:
      return 0;
    }
  }
  double DoubleValue() {
    switch (type_) {
    case INT32:
      return numeric_.int32_t_value;
    case INT64:
      return numeric_.int64_t_value;
    case FLOAT:
      return numeric_.float_value;
    case DOUBLE:
      return numeric_.double_value;
    case STRING:
      double out;
      if (!absl::SimpleAtod(*string_value_, &out)) {
        return 0;
      }
      return out;
      break;
    default:
      return 0;
    }
  }
  std::string StringValue() {
    switch (type_) {
    case INT32:
      return absl::StrCat("", numeric_.int32_t_value);
    case INT64:
      return absl::StrCat("", numeric_.int64_t_value);
    case FLOAT:
      return absl::StrCat("", numeric_.float_value);
    case DOUBLE:
      return absl::StrCat("", numeric_.double_value);
    case STRING:
      return *string_value_;
    // case PROTO:
    //  return (*proto_value_)->DebugString();
    default:
      return "";
    }
  }
  // proto2::Message *ProtoValue() {
  //  switch (type_) {
  //  case PROTO:
  //    return *proto_value_;
  //  default:
  //    return nullptr;
  //  }
  //}

  util::Status Int32Value(int32_t value) {
    switch (type_) {
    case INT32:
      numeric_.int32_t_value = value;
      break;
    case INT64:
      numeric_.int64_t_value = value;
      break;
    case FLOAT:
      numeric_.float_value = value;
      break;
    case DOUBLE:
      numeric_.double_value = value;
      break;
    case STRING:
      string_value_ = absl::StrCat("", value);
      break;
    default:
      return util::Status(
          util::StatusCode::INVALID_ARGUMENT,
          absl::Substitute("cannot assign int32_t to $0", TypeName(type_)));
    }
    return util::Status::OK();
  }
  util::Status Int64Value(int64_t value) {
    switch (type_) {
    case INT32:
      numeric_.int32_t_value = value;
      break;
    case INT64:
      numeric_.int64_t_value = value;
      break;
    case FLOAT:
      numeric_.float_value = value;
      break;
    case DOUBLE:
      numeric_.double_value = value;
      break;
    case STRING:
      string_value_ = absl::StrCat("", value);
      break;
    default:
      return util::Status(
          util::StatusCode::INVALID_ARGUMENT,
          absl::Substitute("cannot assign int64_t to $0", TypeName(type_)));
    }
    return util::Status::OK();
  }
  util::Status FloatValue(float value) {
    switch (type_) {
    case INT32:
      numeric_.int32_t_value = value;
      break;
    case INT64:
      numeric_.int64_t_value = value;
      break;
    case FLOAT:
      numeric_.float_value = value;
      break;
    case DOUBLE:
      numeric_.double_value = value;
      break;
    case STRING:
      string_value_ = absl::StrCat("", value);
      break;
    default:
      return util::Status(
          util::StatusCode::INVALID_ARGUMENT,
          absl::Substitute("cannot assign float to $0", TypeName(type_)));
    }
    return util::Status::OK();
  }
  util::Status DoubleValue(double value) {
    switch (type_) {
    case INT32:
      numeric_.int32_t_value = value;
      break;
    case INT64:
      numeric_.int64_t_value = value;
      break;
    case FLOAT:
      numeric_.float_value = value;
      break;
    case DOUBLE:
      numeric_.double_value = value;
      break;
    case STRING:
      string_value_ = absl::StrCat("", value);
      break;
    default:
      return util::Status(
          util::StatusCode::INVALID_ARGUMENT,
          absl::Substitute("cannot assign double to $0", TypeName(type_)));
    }
    return util::Status::OK();
  }
  util::Status StringValue(std::string value) {
    switch (type_) {
    case INT32:
      if (!absl::SimpleAtoi(value, &numeric_.int32_t_value)) {
        return util::Status(util::StatusCode::INVALID_ARGUMENT,
                            "unable to parse string");
      }
      break;
    case INT64:
      if (!absl::SimpleAtoi(value, &numeric_.int64_t_value)) {
        return util::Status(util::StatusCode::INVALID_ARGUMENT,
                            "unable to parse string");
      }
      break;
    case FLOAT:
      if (!absl::SimpleAtof(value, &numeric_.float_value)) {
        return util::Status(util::StatusCode::INVALID_ARGUMENT,
                            "unable to parse string");
      }
      break;
    case DOUBLE:
      if (!absl::SimpleAtod(value, &numeric_.double_value)) {
        return util::Status(util::StatusCode::INVALID_ARGUMENT,
                            "unable to parse string");
      }
      break;
    case STRING:
      string_value_ = value;
      break;
    default:
      return util::Status(
          util::StatusCode::INVALID_ARGUMENT,
          absl::Substitute("cannot assign string to $0", TypeName(type_)));
    }
    return util::Status::OK();
  }
  // util::Status ProtoValue(proto2::Message *value) {
  //  switch (type_) {
  //  case STRING:
  //    string_value_ = value->DebugString();
  //    break;
  //  case PROTO:
  //    proto_value_ = value;
  //    break;
  //  default:
  //    return util::Status(
  //        util::StatusCode::INVALID_ARGUMENT,
  //        absl::Substitute("cannot assign int32_t to $0", TypeName(type_)));
  //  }
  //  return util::Status::OK();
  //}

  void ForceInt32Value(int32_t value) {
    type_ = INT32;
    numeric_.int32_t_value = value;
  }
  void ForceInt64Value(int64_t value) {
    type_ = INT64;
    numeric_.int64_t_value = value;
  }
  void ForceFloatValue(float value) {
    type_ = FLOAT;
    numeric_.float_value = value;
  }
  void ForceDoubleValue(double value) {
    type_ = DOUBLE;
    numeric_.double_value = value;
  }
  void ForceStringValue(std::string value) {
    type_ = STRING;
    string_value_ = value;
  }
  // void ForceProtoValue(proto2::Message *value) {
  //  type_ = PROTO;
  //  proto_value_ = value;
  //}

private:
  std::string TypeName(DataType type) {
    switch (type) {
    case INT32:
      return "int32_t";
    case INT64:
      return "int64_t";
    case FLOAT:
      return "float";
    case DOUBLE:
      return "double";
    case STRING:
      return "std::string";
    case PROTO:
      return "proto2::Message*";
    case POINTER:
      return "void*";
    default:
      return "";
    }
  }

  union NumericProperty {
    int32_t int32_t_value;
    int64_t int64_t_value;
    float float_value;
    double double_value;
    void *pointer_value;
  };
  NumericProperty numeric_;
  std::optional<std::string> string_value_;
  // std::optional<proto2::Message *> proto_value_;
  DataType type_;
};

class GraphDatabase {
public:
  virtual ~GraphDatabase() {}

  virtual util::Status RegisterNamespace(std::string property_namespace) = 0;

  virtual util::Status RegisterNodeProperty(std::string property_namespace,
                                            std::string property_name,
                                            Property::DataType) = 0;

  virtual util::Status RegisterEdgeProperty(std::string property_namespace,
                                            std::string property_name,
                                            Property::DataType) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<Property::DataType>
  GetNodePropertyType(std::string property_namespace,
                      std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<Property::DataType>
  GetEdgePropertyType(std::string property_namespace,
                      std::string property_name) = 0;

  virtual util::StatusOr<Property>
  GetNodeProperty(Node id, std::string property_namespace,
                  std::string property_name) = 0;

  virtual util::StatusOr<Property>
  GetEdgeProperty(Edge id, std::string property_namespace,
                  std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int32_t>
  GetNodePropertyInt32(Node id, std::string property_namespace,
                       std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int32_t>
  GetEdgePropertyInt32(Edge id, std::string property_namespace,
                       std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int64_t>
  GetNodePropertyInt64(Node id, std::string property_namespace,
                       std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<int64_t>
  GetEdgePropertyInt64(Edge id, std::string property_namespace,
                       std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<float>
  GetNodePropertyFloat(Node id, std::string property_namespace,
                       std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<float>
  GetEdgePropertyFloat(Edge id, std::string property_namespace,
                       std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<double>
  GetNodePropertyDouble(Node id, std::string property_namespace,
                        std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<double>
  GetEdgePropertyDouble(Edge id, std::string property_namespace,
                        std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<std::string>
  GetNodePropertyString(Node id, std::string property_namespace,
                        std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::StatusOr<std::string>
  GetEdgePropertyString(Edge id, std::string property_namespace,
                        std::string property_name) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetNodePropertyInt32(Node id,
                                            std::string property_namespace,
                                            std::string property_name,
                                            int32_t property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetEdgePropertyInt32(Edge id,
                                            std::string property_namespace,
                                            std::string property_name,
                                            int32_t property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetNodePropertyInt64(Node id,
                                            std::string property_namespace,
                                            std::string property_name,
                                            int64_t property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetEdgePropertyInt64(Edge id,
                                            std::string property_namespace,
                                            std::string property_name,
                                            int64_t property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetNodePropertyFloat(Node id,
                                            std::string property_namespace,
                                            std::string property_name,
                                            float property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetEdgePropertyFloat(Edge id,
                                            std::string property_namespace,
                                            std::string property_name,
                                            float property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetNodePropertyDouble(Node id,
                                             std::string property_namespace,
                                             std::string property_name,
                                             double property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetEdgePropertyDouble(Edge id,
                                             std::string property_namespace,
                                             std::string property_name,
                                             double property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetNodePropertyString(Node id,
                                             std::string property_namespace,
                                             std::string property_name,
                                             std::string property) = 0;

  // Not required. Will return kUnimplemented if not implemented.
  virtual util::Status SetEdgePropertyString(Edge id,
                                             std::string property_namespace,
                                             std::string property_name,
                                             std::string property) = 0;
};

class GraphProperties {
public:
  virtual ~GraphProperties() {}

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

class Diagram {
public:
  virtual ~Diagram() {}

  virtual util::Status GenerateDiagram(std::string diagram_name) = 0;
};

class SVGGraphDiagrammer {
public:
  virtual ~SVGGraphDiagrammer() {}

  virtual util::StatusOr<std::string> GenerateDiagram(Diagram *diagram) = 0;
};

#endif // EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_GRAPH_H_
