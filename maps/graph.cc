#include "graph.h"

#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <stdlib.h>
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
      break;
    case EAST:
      return "E";
      break;
    case SOUTH:
      return "S";
      break;
    case WEST:
      return "W";
      break;
    case ERROR:
      return "ERROR";
      break;
    default:
      return "UNKOWN DIRECTION";
  }
}

// Returns the number of unparseable edges (or -1 if error opening the file);
int Graph::BuildFromCSVFile(const std::string& filename) {
  std::ifstream file(filename.c_str());
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
  const float length = atof(tokens[0].c_str());
  const int from_cross_street_id = static_cast<int>(atof(tokens[1].c_str()));
  const std::string &street_name = tokens[3] + " " + tokens[2] + " " +
    tokens[10];
  const std::string &zip_code = tokens[4];
  const int from_node_id = static_cast<int>(atof(tokens[5].c_str()));
  const std::string &dir_travel = tokens[6]; // TODO(paolo): actually encode directionality.
  const std::string &from_cross_street = tokens[7];
  const std::string &censusubl = tokens[8];
  const int to_node_id = static_cast<int>(atof(tokens[9].c_str()));
  const std::string &street_type = tokens[10];
  const CardinalDirection ewns_dir = CardinalDirectionFromString(tokens[11]);
  // b = beginning, e = end for latlong?
  const double &blong = atof(tokens[12].c_str());
  const double &elong = atof(tokens[13].c_str());
  const double &blat = atof(tokens[14].c_str());
  const double &elat = atof(tokens[15].c_str());

  // Create the edge and populate values.
  const Edge edge = {from_node_id, to_node_id, length, ewns_dir,
                     street_name + " & " + from_cross_street};
  const LatLong from_latlong = {blat, blong};
  if (!AddNodeIfNotPresent(from_node_id, from_latlong,
			   street_name + " & " + from_cross_street))
    return false;
  nodes_[from_node_id].edges_.push_back(edge);
  const LatLong to_latlong = {elat, elong};
  if (!AddNodeIfNotPresent(to_node_id, to_latlong,
			   street_name + " & " + from_cross_street))
    return false;
  // Add to_edge and inverse if needed.
  Node *to_node = &nodes_[to_node_id];
  if (!to_node->HasEdgeTo(from_node_id)) {
    const Edge reverse_edge = {to_node_id, from_node_id, length, ewns_dir,
			       "I:" + street_name + " & " + from_cross_street};
    to_node->edges_.push_back(reverse_edge);
  }
  return true;
}

std::string LatLongDebugString(LatLong latlong) {
  return "(" + std::to_string(latlong.lat) + "," +
    std::to_string(latlong.lon) + ")";
}

bool Graph::AddNodeIfNotPresent(int node_id, LatLong latlong,
				std::string name) {
  // Check if the from_node already exists. If not, create it.
  if (node_id > kMaxNumNodes || node_id < 0) return false;

  if ((unsigned)node_id > nodes_.size()) {
    nodes_.resize(node_id + 1);
  }
  Node *node = &nodes_[node_id];
  if (node->index_ == -1) {  // This node is not inialized yet!
    node->Initialize(node_id, latlong, name);
  }// else if (node->latlong_.lat != latlong.lat ||
  //	     node->latlong_.lon != latlong.lon) {
  //    node->other_latlongs_ = node->other_latlongs_ + LatLongDebugString(latlong);
  //  }
  return true;
}

bool Node::HasEdgeTo(const int id) const {
  for (int i = 0; i < edges_.size(); ++i) 
    if (edges_[i].to_id == id) return true;
  return false;
}

void Node::Print(const bool verbose) const {
  if (verbose) {
    std::cout << "Node index " << index_ << " :" << std::endl;
    std::cout << " <" << std::endl;
    std::cout << "  intersection: " << intersection_ << std::endl;
    std::cout << "  latlong: " << latlong_.lat << "," << latlong_.lon << std::endl;
    for (int i = 0; i < edges_.size(); ++i) PrintEdge(edges_[i], verbose, 2);
    std::cout << " >" << std::endl;
  } else {
    std::cout << "idx " << index_
	      << "(" << latlong_.lat << "," << latlong_.lon << "):";
    for (int i = 0; i < edges_.size(); ++i) PrintEdge(edges_[i], verbose, 2);
    std::cout << std::endl;
  }
}

void PrintEdge (const Edge &edge, const bool verbose, int offset) {
  const std::string off (offset, ' ');
  if (verbose) {
    std::cout << off << edge.from_id << " -> " << edge.to_id << std::endl;
    std::cout << off << "  <" << std::endl;
    std::cout << off << "   length: " << edge.length << std::endl;
    std::cout << off << "   direction: "
	      << CardinalDirectionToString(edge.direction) << std::endl;
    std::cout << off << "   street_name: " << edge.street_name << std::endl;
    std::cout << off << "  >" << std::endl;
  } else {
    std::cout << off << edge.from_id << "->" << edge.to_id << "{" 
	      << edge.street_name << ", len: " << edge.length << " dir: "
	      << CardinalDirectionToString(edge.direction) << "} ";
  }
}

void Graph::PrintInternal(const bool verbose, int max_num) const {
  if (max_num == -1 || max_num > nodes_.size())
    max_num = nodes_.size();
  for (int i = 0; i < max_num; ++i) {
    if (nodes_[i].index_ != -1) nodes_[i].Print(verbose);
  }
}

void Graph::PrintStats(bool verbose) const {
  std::cout << "Number of vertices: " << nodes_.size() << std::endl;
  int num_uninitialized_vertices = 0;
  int num_nodes_multiple_latlongs = 0;
  int num_edges = 0;
  std::map<int, int> degree_hist;
  int max_degree = -1;
  int max_degree_index = 0; 
  for (const auto& node : nodes_) {
    if (node.index_ == -1) {
      ++num_uninitialized_vertices;
      continue;
    }
    if (!node.other_latlongs_.empty()) {
      ++num_nodes_multiple_latlongs;
    }
    const int& degree = node.edges_.size();
    degree_hist[degree]++;
    num_edges += degree;
    if (degree > max_degree) {
      max_degree = degree;
      max_degree_index = node.index_;
    }
  }
  std::cout << "Number of good (initalized) vertices: "
	    << nodes_.size() - num_uninitialized_vertices << std::endl;
  std::cout << "Number of vertices with multiple latlongs: "
	    << num_nodes_multiple_latlongs << std::endl;
  std::cout << "Number of edges: " << num_edges << std::endl;
  std::cout << "Maximum degree: " << max_degree << std::endl;
  if (verbose) {
    std::cout << "max_degree vertex: ";
    nodes_[max_degree_index].Print();
  }
  //  std::cout << "median_degree" << GetMedianFromHist(degree_hist);
  if (verbose) {
    std::cout << "Full degree histogram: degree -> num_nodes: " << std::endl;
    for (std::map<int, int>::iterator it = degree_hist.begin(); 
	 it!= degree_hist.end(); ++it) {
      std::cout << it->first << " --> " << it->second << std::endl;
    }
  }
}

}  // namespace graph
