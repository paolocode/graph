// Simple graph class.

#include <vector>
#include <string>

namespace graph {

const int kMaxNumNodes = 1000000;

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

void PrintEdge (const Edge &edge, const bool verbose, int offset = 0);

class Node {
public:
  Node() { index_ = -1; }
  void Initialize(const int index, const LatLong &latlong,
		  const std::string &intersection) {
    index_ = index; 
    latlong_ = latlong;
    intersection_ = intersection;
  }
  bool HasEdgeTo(const int id) const;
  void Print(const bool verbose = false) const;
  int index_;
  LatLong latlong_;
  // Can we get this in canonical form?
  std::string intersection_;
  // Indices of neighbors in the nodes vector in Graph.
  std::vector<Edge> edges_;
  // Should this really live inside the Node class?
  std::vector<int> neighborhood_30_min_;
  std::string other_latlongs_;
};

class Graph {
 public:
  ~Graph() { };
  int BuildFromCSVFile(const std::string &filename);
  void PrintVerbose(int max_num = kMaxNumNodes) const {
    PrintInternal(/*verbose =*/true, max_num);
  }
  void Print(int max_num = kMaxNumNodes) const {
    PrintInternal(/*verbose =*/false, max_num);
  } 
  void PrintStats(bool verbose = false) const;
 private:
  void PrintInternal(const bool verbose, int max_num) const;
  bool AddEdge(const std::vector<std::string> &tokens);
  bool AddNodeIfNotPresent(int node_id, LatLong lat_long,
			   std::string name);
  std::vector<Node> nodes_;
};

}  // namespace graph
