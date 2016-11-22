#include "graph.h"

#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "../utils/csv_util.h"

namespace graph {

CardinalDirection CardinalDirectionFromString(const std::string& string) {
  if (string == "N") return NORTH;
  if (string == "E") return EAST;
  if (string == "S") return SOUTH;
  if (string == "W") return WEST;
  return ERROR;
}

std::string CardinalDirectionToString(const CardinalDirection cd) {
  switch (cd) {
    case NORTH:
      return "N";
    case EAST:
      return "E";
    case SOUTH:
      return "S";
    case WEST:
      return "W";
    case ERROR:
      return "ERROR";
    default:
      return "UNKOWN DIRECTION";
  }
}

// Returns the number of unparseable edges (or -1 if error opening the file);
int Graph::BuildFromCSVFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "FAILED TO OPEN FILE" << std::endl;
    return -1;
  }
  int num_unparsable_lines = 0;
  // Ignore the first line since it's just the names.
  std::vector<std::string> tokens = getNextLineAndSplitIntoTokens(file);
  while (!tokens.empty()) {
    std::vector<std::string> tokens = getNextLineAndSplitIntoTokens(file);
    if (tokens.empty()) break;
    if (!AddEdge(tokens)) {
      num_unparsable_lines++;
    }
  }
  return num_unparsable_lines;
}

// Format:
// length (float), from_cross_street (float), street_name (string),
// pre_dir (CardinalDirection string, I think this is the direction of the street.),
// zip_code (int), f_node_id (float), dir_travel (F/B), f_cross (string), 
// censusubl (string? very long int), to_node_id (float), street type (string),
// ewns_dir (CardinalDirection string),
// BLONG (double?), ELONG(double?), BLAT(double?), ELAT(double?)
bool Graph::AddEdge(const std::vector<std::string> &tokens) {
  if (tokens.size() != 16) return false;
  // Parse all tokens (we could put them directly in, but this way we know what they are.
  const float length = std::stof(tokens[0]);
  const int from_cross_street_id = static_cast<int>(std::stof(tokens[1]));
  const std::string &street_name = tokens[3] + " " + tokens[2];
  const std::string &zip_code = tokens[4];
  const int from_node_id = static_cast<int>(std::stof(tokens[5]));
  const std::string &dir_travel = tokens[6]; // TODO(paolo): actually encode directionality.
  const std::string &from_cross_street = tokens[7];
  const std::string &censusubl = tokens[8];
  const int to_node_id = static_cast<int>(std::stof(tokens[9]));
  const std::string &street_type = tokens[10];
  const CardinalDirection ewns_dir = CardinalDirectionFromString(tokens[11]);
  // b = beginning, e = end for latlong?
  const double &blong = std::stod(tokens[12]);
  const double &elong = std::stod(tokens[13]);
  const double &blat = std::stod(tokens[14]);
  const double &elat = std::stod(tokens[15]);

  // Create the edge and populate values.
  const Edge edge = {from_node_id, to_node_id, length, ewns_dir};
  // Check if the from_node already exists. If not, create it.
  if (from_node_id > nodes_.size()) {
    nodes_.resize(from_node_id + 1);
  }
  Node *node = &nodes_[from_node_id];
  if (node->index_ == -1) {  // This node is not inialized yet!
    const LatLong from_latlong = {blat, blong};
    node->Initialize(from_node_id, from_latlong,
		     street_name + " & " + from_cross_street);
  }
  node->edges_.push_back(edge);
  return true;
}

void PrintEdge (const Edge &edge, int offset = 0) {
  const std::string off (offset, ' ');
  std::cout << off << edge.from_id << " -> " << edge.to_id << std::endl;
  std::cout << off << "  <" << std::endl;
  std::cout << off << "   length: " << edge.length << std::endl;
  std::cout << off << "   direction: " << CardinalDirectionToString(edge.direction) << std::endl;
  std::cout << off << "   street_name: " << edge.street_name << std::endl;
  std::cout << off << "  >" << std::endl;
}

}  // namespace graph
