#include "csv_util.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool CheckEqualsString(const std::string& str1, const std::string& str2) {
  if (str1 != str2) {
    std::cerr << "Expected strings: '" << str1 << "' == '" << str2 << "'" << std::endl;
    return false;
  }
  return true;
}

bool CheckEqualsInt(const int i1, const int i2) {
  if (i1 != i2) {
    std::cerr << "Expected Ints: " << i1 << " == " << i2 << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  bool test_passed = true;
  std::ifstream file("test.csv");
  if (!file.is_open()) {
    std::cerr << "FAILED TO OPEN FILE" << std::endl;
    return 2;
  }
  std::vector<std::string> tokens = getNextLineAndSplitIntoTokens(file);
  if (!CheckEqualsInt(tokens.size(), 4)) test_passed = false;
  if (!CheckEqualsString(tokens[0], "I am") ||
      !CheckEqualsString(tokens[1], " a test") ||
      !CheckEqualsString(tokens[2], " csv") ||
      !CheckEqualsString(tokens[3], " file")) test_passed = false;

  tokens = getNextLineAndSplitIntoTokens(file);
  if (!CheckEqualsInt(tokens.size(), 3)) test_passed = false;
  if (!CheckEqualsString(tokens[0], "I am") ||
      !CheckEqualsString(tokens[1], " number 1") ||
      !CheckEqualsString(tokens[2], " test!")) test_passed = false;

  if (test_passed) {
    std::cout << "SUCCESS!" << std::endl;
    return 0;
  } else  {
    std::cout << "TEST FAILED!" << std::endl;
    return 2;
  }
}
