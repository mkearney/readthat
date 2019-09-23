#include <fstream>
#include <regex>
#include <sstream>
#include <cstdlib>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <Rcpp.h>
#include <iostream>

// [[Rcpp::plugins(cpp17)]]


#include <experimental/filesystem>


namespace fs = std::experimental::filesystem;

using namespace Rcpp;


std::string path_home() {
  const char * home = getenv ("HOME");
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

std::string path_absolute_(std::string path = ".") {
  fs::path p = fs::absolute(path_expand_(path));
  return p.u8string();
}

// [[Rcpp::export]]
CharacterVector readthatcpp(std::string path) {

  // if URL then use curlpp
  std::regex pat("(http)(.*)");
  if (std::regex_match(path, pat)) {
    std::ostringstream os;
    curlpp::Easy req;
    req.setOpt(curlpp::options::Url(path));
    os << req;
    return os.str();
  }

  // otherwise read local file
  path = path_absolute_(path);
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

// [[Rcpp::export]]
CharacterVector readthatcpp2(std::string path) {

  // if URL then use curlpp
  std::regex pat("(http)(.*)");
  if (std::regex_match(path, pat)) {
    std::ostringstream os;
    curlpp::Easy req;
    req.setOpt(curlpp::options::Url(path));
    os << req;
    return os.str();
  }

  // otherwise read local file
  Function normalize_path("normalizePath");
  path = as<std::string>(normalize_path(path));
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


// [[Rcpp::export]]
std::string path_current() {
  fs::path p = fs::current_path();
  return p.u8string();
}

// [[Rcpp::export]]
std::string path_absolute(std::string path = ".") {
  fs::path p = fs::absolute(path_expand_(path));
  return p.u8string();
}

// [[Rcpp::export]]
std::string path_canonical(std::string path = ".") {
  fs::path p = fs::canonical(path_expand_(path));
  return p.u8string();
}

// [[Rcpp::export]]
std::string path_relative(std::string path = ".") {
  fs::path p = path;
  return p.u8string();
}

// [[Rcpp::export]]
std::string path_expand(std::string path) {
  std::regex pat(".*~.*");
  if (std::regex_match(path, pat)) {
    std::regex pat(".*~");
    path = std::regex_replace(path, pat, path_home());
  }
  return path;
}

// [[Rcpp::export]]
std::string path_copy(std::string from, std::string to, bool recursive = false) {
  if (recursive) {
    fs::copy(path_expand_(from), path_expand_(to), fs::copy_options::recursive);
  } else {
    fs::copy(path_expand_(from), path_expand_(to));
  }
  return path_expand_(to);
}


// [[Rcpp::export]]
bool directory_create(std::string dir) {
  return fs::create_directory(path_expand_(dir));
}


// [[Rcpp::export]]
bool path_exists(std::string path) {
  return fs::exists(path_expand_(path));
}

// [[Rcpp::export]]
bool path_remove(std::string path) {
  return fs::remove(path_expand_(path));
}

// [[Rcpp::export]]
std::string tmp_dir() {
  fs::path dir = fs::temp_directory_path();
  return dir;
}

// [[Rcpp::export]]
std::string tmp_path() {
  return std::tmpnam(nullptr);
}

// [[Rcpp::export]]
std::string current_root() {
  fs::path p = fs::current_path().root_path();
  return p.u8string();
}


DataFrame as_tbl_(List x) {
  x.attr("class") = CharacterVector::create("tbl_df", "tbl", "data.frame");
  return x;
}


// [[Rcpp::export]]
int path_size(std::string path) {
  return fs::file_size(path_expand_(path));
}

// [[Rcpp::export]]
DataFrame path_space(std::string path) {
  fs::space_info s = fs::space(path_expand_(path));
  DataFrame d = DataFrame::create(Named("capacity") = s.capacity , _["free"] = s.free, _["available"] = s.available);
  return as_tbl_(d);
}

// [[Rcpp::export]]
DataFrame as_tbl(List x) {
  return as_tbl_(x);
}

