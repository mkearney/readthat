#include <fstream>
#include <regex>
#include <Rcpp.h>

using namespace Rcpp;

// [[Rcpp::export]]
CharacterVector readthatcpp(CharacterVector path) {
  std::string fname = as<std::string>(path);
  std::regex pat("(http)(.*)");
  if (std::regex_match(fname, pat)) {
    Function readweb("readweb");
    return readweb(fname);
  }
  Function normalize_path("normalizePath");
  fname = as<std::string>(normalize_path(fname));
  std::ifstream in(fname.c_str());
  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();
  return contents;
}


