#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export]]
std::vector<std::string> readlcpp(std::string filename, const int first = 1, const int last = 0) {

  // Create string vector
  std::vector<std::string> vecstr;

  // Open the File
  std::ifstream inp(filename.c_str());

  // Initialize counter and check last bool
  int i = 1;

  // Initialize line value
  std::string str;

  // If all lines should be read
  if (last < 1) {
    while (std::getline(inp, str)) {
      if (i < first) {
        i = i + 1;
        continue;
      }
      if (str.size() == 0)
        str = "";
      vecstr.push_back(str);
    }
    inp.close();
    return vecstr;
  }

  // loop through lines
  while (std::getline(inp, str)) {
    if (i < first) {
      // update counter
      i = i + 1;
      continue;
    }

    // if line has zero length replace with empty string
    if (str.size() == 0)
      str = "";

    // add to vector
    vecstr.push_back(str);

    // update counter
    i = i + 1;

    // if last line has been reached then close and return
    if (i > last) {
      inp.close();
      return vecstr;
    }
  }

  // close and return
  inp.close();
  return vecstr;
}
