#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <future>
#include <string>
#include <cstdio>
//#include <vector>

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
StringVector readthatcpp(std::string path) {

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
    StringVector content = os.str();
    content.attr("names") = path;
    return content;
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
  StringVector out = StringVector::create(content);
  out.attr("names") = path;
  return out;
}

std::string readit(std::string path) {
  std::ifstream in(path);
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
StringVector readthosecpp2(std::vector<std::string> paths) {
  const int n = paths.size();
  StringVector lst(n);
  for (int i = 0; i < n; ++i) {
    std::regex pat("(http)(.*)");
    if (std::regex_match(paths[i], pat)) {
      std::future<std::string> x = invoke(paths[i]);
      lst[i] = x.get();
    } else {
      lst[i] = readit(paths[i]);
    }
  }
  lst.attr("names") = paths;
  return lst;
}




void readone(const std::string& url, const std::string& out) {
  std::ofstream of(out);
  cURLpp::Easy req;
  req.setOpt(cURLpp::Options::Url(url));
  req.setOpt(cURLpp::Options::NoProgress(true));
  req.setOpt(cURLpp::Options::FollowLocation(true));
  req.setOpt(cURLpp::Options::WriteFunction([&](const char* p, std::size_t size, std::size_t nmemb) {
    of.write(p, size*nmemb);
    return size*nmemb;
  }));
  req.perform();
}



// [[Rcpp::export]]
StringVector readthosecpp(std::vector<std::string> paths) {
  std::vector<std::string> outs(paths.size());
  cURLpp::initialize();
  for (int i = 0; i < paths.size(); ++i) {
    // if URL
    std::regex pat("(http)(.*)");
    if (std::regex_match(paths[i], pat)) {
      char name[L_tmpnam];
      outs[i] = std::tmpnam(name);
      readone(paths[i], outs[i]);
    } else {
      outs[i] = paths[i];
    }
  }
  StringVector contents(paths.size());
  for (int i = 0; i < outs.size(); ++i) {
    contents[i] = readit(outs[i]);
  }
  contents.attr("names") = paths;
  return contents;
}







// [[Rcpp::export]]
StringVector downloadthosecpp(std::vector<std::string> urls, std::vector<std::string> outs) {
  cURLpp::initialize();
  std::vector<std::thread> dwnld;
  for (int i = 0; i < urls.size(); ++i) {
    dwnld.emplace_back([p = urls[i], o = outs[i]]{
      readone(p, o);
    });
  }
  for(auto& p: dwnld){
    p.join();
  }
  StringVector out(outs.size());
  for (int i = 0; i < urls.size(); ++i) {
    out[i] = outs[i];
  }
  out.attr("names") = urls;
  return out;
}

