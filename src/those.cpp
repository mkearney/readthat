
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include <thread>
#include <vector>
#include <mutex>

#include <Rcpp.h>
using namespace Rcpp;

using namespace std::literals;

namespace {

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

// [[Rcpp::export]]
StringVector readthosecpp(std::vector<std::string> urls) {
  std::vector<std::string> outs(urls.size());
  for (int i = 0; i < urls.size(); ++i) {
    char name[L_tmpnam];
    outs[i] = std::tmpnam(name);
  }
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
  //std::vector<std::string> contents(urls.size());
  StringVector contents(urls.size());

  for (int i = 0; i < outs.size(); ++i) {
    contents[i] = readit(outs[i]);
  }
  contents.attr("names") = urls;
  return contents;
}


// [[Rcpp::export]]
std::vector<std::string> downloadthosecpp(std::vector<std::string> urls, std::vector<std::string> outs) {
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
  return outs;
}


