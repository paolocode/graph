// Copied from: 
// http://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
#include "csv_util.h"

#include <istream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str) {
  std::vector<std::string> result;
  std::string line;
  std::getline(str, line);
  
  std::stringstream lineStream(line);
  std::string cell;
  
  while(std::getline(lineStream, cell, ',')) {
    result.push_back(cell);
  }
  return result;
}
