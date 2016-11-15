// Copied from: 
// http://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c

#ifndef UTILS_CSV_UTIL_H_
#define UTILS_CSV_UTIL_H_

#include <istream>
#include <string>
#include <vector>

std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str);

#endif  // UTILS_CSV_UTIL_H_
