// Simple graph class. TODO: Consider using a class instead?

#include <vector>

namespace graph {

enum CardinalDirections {
  NORTH = 0;
  EAST = 1;
  SOUTH = 2;
  WEST = 3;
};

struct LatLong {
  float lat;
  float lon;
};

struct HalfEdgeData {
  int index;
  LatLong latlong;
  string intersection;
}

class Edge {
 public:
  ~Edge() { }
  HalfEdgeData source;
  HalfEdgeData dest;
  float length;
  CardinalDirection direction;
  sting street_name;
};

class Node {
 public:
  int index;
  LatLong latlong;
  // Can we get this in canonical form?
  string intersection;
  // Indices of neighbors in the nodes vector in Graph.
  std::vector<Edge> edges;
  // Should this really live inside the Node class?
  std::vector<int> 30-min-neighbors;
};

class Graph {
 public:
  ~Graph() { };
  BuildFromCSVFile(const string& filename);
 private:
  AddEdge(const string& csv_edge_line);
  std::Vector<GraphNode> nodes;
};

}  // namespace graph
