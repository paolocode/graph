// Simple graph class.

#include <vector>
#include <string>

namespace graph {

enum CardinalDirection {
  ERROR = 0,
  NORTH = 1,
  EAST = 2,
  SOUTH = 3,
  WEST = 4,
};

typedef struct {
  double lat;
  double lon;
} LatLong;

typedef struct {
  int from_id;
  int to_id;
  float length;
  CardinalDirection direction;
  std::string street_name;
} Edge;

class Node {
public:
  Node() { index_ = -1; }
  void Initialize(const int index, const LatLong &latlong, const std::string &intersection) {
    index_ = index;
    latlong_ = latlong;
    intersection_ = intersection;
  }
  int index_;
  LatLong latlong_;
  // Can we get this in canonical form?
  std::string intersection_;
  // Indices of neighbors in the nodes vector in Graph.
  std::vector<Edge> edges_;
  // Should this really live inside the Node class?
  std::vector<int> neighborhood_30_min;
};

class Graph {
 public:
  ~Graph() { };
  int BuildFromCSVFile(const std::string &filename);
 private:
  bool AddEdge(const std::vector<std::string> &tokens);
  std::vector<Node> nodes_;
};

}  // namespace graph
