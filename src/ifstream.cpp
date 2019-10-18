#include <Rcpp.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace Rcpp;

// [[Rcpp::export]]
std::string readcpp(const std::string& path) {
  std::ifstream inp(path.c_str());
  std::string content;
  inp.seekg(0, std::ios::end);
  content.resize(inp.tellg());
  inp.seekg(0, std::ios::beg);
  inp.imbue( std::locale() );
  inp.read(&content[0], content.size());
  inp.close();
  return content;
}
