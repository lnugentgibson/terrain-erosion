#ifndef EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_FORCE_DIAGRAM_H_
#define EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_FORCE_DIAGRAM_H_

#include "experimental/users/nugentgibson/graph/graph.h"

enum PlacementType { PLACEMENT_RANDOM, PLACEMENT_CIRCLE };

struct PlacementOptions {
  PlacementType placement_type;
  absl::optional<float> param1;

  PlacementOptions() : placement_type(PLACEMENT_CIRCLE), param1(5) {}

  bool IsValid() const {
    switch (placement_type) {
    case PLACEMENT_CIRCLE:
      return param1.has_value() && *param1 > 0.001;
    default:
      return true;
    }
  }
};

struct StopCondition {
  absl::optional<float> velocity_threshold;
  absl::optional<float> energy_threshold;
  absl::optional<int> step_threshold;

  StopCondition() : energy_threshold(0.1) {}

  bool IsValid() const {
    return velocity_threshold.has_value() || energy_threshold.has_value() ||
           step_threshold.has_value();
  }
};

struct FrictionOptions {
  absl::optional<float> static_friction;
  absl::optional<float> kinetic_friction;
  absl::optional<float> drag;
};

struct GravityOptions {
  float center_mass;
  float strength;
};

enum BoundaryType { BOUNDARY_SQUARE, BOUNDARY_CIRCLE };

struct BoundaryOptions {
  absl::optional<int> boundary_type;
  absl::optional<std::pair<float, float>> size;
  float strength;

  BoundaryOptions()
      : boundary_type(BOUNDARY_CIRCLE), size({100, 100}), strength(5) {}
};

struct ForceOptions {
  float time_step;
  absl::optional<FrictionOptions> friction;
  absl::optional<GravityOptions> gravity;
  float electricity;
  absl::optional<BoundaryOptions> boundary;

  ForceOptions() : time_step(0.01), electricity(1) {}
};

struct GenerationOptions {
  absl::optional<bool> save_intermediate;
};

struct ForceDiagramOptions {
  PlacementOptions placement;
  StopCondition stop_condition;
  ForceOptions forces;
  absl::optional<GenerationOptions> generation;

  inline bool IsValid() const {
    return placement.IsValid() && stop_condition.IsValid();
  }
};

struct NodeState {
  float mass;
  float charge;
  float p_x, p_y;
  float v_x, v_y;
  float a_x, a_y;
  NodeState() : NodeState(0, 0, 1) {}
  NodeState(float x, float y, float m = 1, float c = 1)
      : mass(m), charge(c), p_x(x), p_y(y), v_x(0), v_y(0), a_x(0), a_y(0) {}
};

struct EdgeState {
  float neutral_length;
  float tension;
  NodeState *a, *b;
  float n_x, n_y;
  float f_x, f_y;
  EdgeState(NodeState *start, NodeState *end, float l = 10, float t = 0.1)
      : neutral_length(l), tension(t), a(start), b(end) {}
  EdgeState(const EdgeState &state)
      : neutral_length(state.neutral_length), tension(state.tension),
        a(state.a), b(state.b) {}
  std::pair<float, float> Calculate(ForceDiagramOptions options);
};

class ForceDiagram : public Diagram {
public:
  explicit ForceDiagram(ForceDiagramOptions options);

  absl::Status GenerateDiagram(std::string diagram_name) override;

  absl::Status Initialize();

  util::StatusOr<bool> Step(int steps);

private:
  Graph *graph_;
  GraphDatabase *database_;
};

#endif // EXPERIMENTAL_USERS_NUGENTGIBSON_GRAPH_FORCE_DIAGRAM_H_
