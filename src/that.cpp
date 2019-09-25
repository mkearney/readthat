#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <future>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

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
    curlpp::Cleanup myCleanup;
    cURLpp::Easy req;
    req.setOpt(cURLpp::Options::Url(path));
    req.setOpt(cURLpp::Options::NoProgress(true));
    req.setOpt(cURLpp::Options::FollowLocation(true));
    std::ostringstream os;
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

std::future<std::string> invoke(std::string const& url) {
  return std::async(std::launch::async,
    [](std::string const& url) mutable {

      curlpp::Cleanup clean;
      curlpp::Easy r;
      r.setOpt(new curlpp::options::Url(url));

      std::ostringstream response;
      r.setOpt(new curlpp::options::WriteStream(&response));

      r.perform();

      return std::string(response.str());
    }, url);
}


// [[Rcpp::export]]
std::vector<std::string> readthosecpp2(std::vector<std::string> urls) {
  int n = urls.size();
  std::vector<std::string> lst = std::vector<std::string>(n);
  for (int i = 0; i < n; ++i) {
    std::future<std::string> x = invoke(urls[i]);
    lst[i] = x.get();
  }
  return lst;
}

