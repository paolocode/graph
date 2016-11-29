#include "graph.h"

#include <iostream>

bool CheckEqualsInt(const int i1, const int i2) {
  if (i1 != i2) {
    std::cerr << "Expected Ints: " << i1 << " == " << i2 << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  graph::Graph graph;
  int num_unreadable_lines = graph.BuildFromCSVFile("../data/full.csv");
  CheckEqualsInt(num_unreadable_lines, 0);
  graph.PrintStats(true);
  graph.Print();
  return 0;
}
