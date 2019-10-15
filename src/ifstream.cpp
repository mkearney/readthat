#include <Rcpp.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#include <regex>

using namespace Rcpp;

std::string path_home() {
  const char * home = getenv("HOME");
  return home;
}


// [[Rcpp::export]]
std::string readcpp(std::string& path) {
  std::regex pat(".*~.*");
  if (std::regex_match(path, pat)) {
    std::regex pat(".*~");
    path = std::regex_replace(path, pat, path_home());
  }
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



