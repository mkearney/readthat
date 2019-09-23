#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include <Rcpp.h>

using namespace Rcpp;


std::string path_home() {
  const char * home = getenv("HOME");
  return home;
}

std::string path_expand_(std::string path) {
  std::regex pat(".*~.*");
  if (std::regex_match(path, pat)) {
    std::regex pat(".*~");
    path = std::regex_replace(path, pat, path_home());
  }
  return path;
}

// [[Rcpp::export]]
CharacterVector readthatcpp(std::string path) {

  // if URL then use curlpp
  std::regex pat("(http)(.*)");
  if (std::regex_match(path, pat)) {
    std::ostringstream os;
    curlpp::Easy req;
    req.setOpt(curlpp::options::Url(path));
    cURLpp::initialize();
    os << req;
    cURLpp::terminate();
    return os.str();
  }

  // otherwise read local file
  path = path_expand_(path);
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

