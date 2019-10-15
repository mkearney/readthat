#include <Rcpp.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace Rcpp;

// [[Rcpp::export]]
std::string readcpp(const std::string& path) {
  std::ifstream in(path.c_str());
  std::string content;
  in.seekg(0, std::ios::end);
  content.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.imbue( std::locale() );
  in.read(&content[0], content.size());
  in.close();
  return content;
}



