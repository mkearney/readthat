
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include <thread>
#include <vector>
#include <mutex>

#include <Rcpp.h>
using namespace Rcpp;

using namespace std::literals;

namespace {

void Download(const std::string& url, unsigned line, const std::string& out) {
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
std::vector<std::string> readthemcpp(std::vector<std::string> urls, std::vector<std::string> outs) {
  cURLpp::initialize();
  unsigned line = 1;

  std::vector<std::thread> currDownloading;

  //for(const auto& p: urls) {
  for (int i = 0; i < urls.size(); ++i) {
    line = line + 1;
    currDownloading.emplace_back([p = urls[i], l = line, o = outs[i]]{
      Download(p, l, o);
    });
  }
  for(auto& p: currDownloading){
    p.join();
  }
  std::vector<std::string> contents(urls.size());
  for (int i = 0; i < outs.size(); ++i) {
    contents[i] = readit(outs[i]);
  }
  return contents;
}


