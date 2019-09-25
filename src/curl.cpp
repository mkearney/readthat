#include <Rcpp.h>
#include <curl/curl.h>
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

std::string path_expand_(std::string path) {
  std::regex pat(".*~.*");
  if (std::regex_match(path, pat)) {
    std::regex pat(".*~");
    path = std::regex_replace(path, pat, path_home());
  }
  return path;
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



static std::size_t write_data_string(void *ptr,
                                     std::size_t size,
                                     std::size_t nmemb,
                                     void *stream) {
  std::string data((const char*) ptr, (std::size_t) size * nmemb);
  *((std::stringstream*) stream) << data << std::endl;
  return size * nmemb;
}


std::string readthatcpp_(std::string path) {
  std::regex pat("(http)(.*)");
  if (std::regex_match(path, pat)) {
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *handle = curl_easy_init();
    if(handle == NULL) {
      fprintf(stderr, "curl_easy_init() failed\n");
    }
    CURLcode code = curl_easy_setopt(handle, CURLOPT_URL, path.c_str());
    if(code != CURLE_OK) {
      fprintf(stderr, "curl_easy_setopt() failed: %s\n",
        curl_easy_strerror(code));
    }
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
    // Prevent "longjmp causes uninitialized stack frame" bug
    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "deflate");
    std::stringstream out;
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data_string);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &out);
    // Make request (res = return code)
    CURLcode res = curl_easy_perform(handle);
    // Check for errors
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));
    }
    curl_easy_cleanup(handle);
    curl_global_cleanup();
    return out.str();
  }
  return readit(path_expand_(path));
}

static std::size_t write_data_file(void *ptr,
                                   std::size_t size,
                                   std::size_t nmemb,
                                   void *stream) {
  std::size_t written = std::fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

std::string downloadthatcpp_(const std::string& url,
                             const std::string& out) {
  const bool verbose = false;
  CURL *handle;
  FILE *outfile;

  curl_global_init(CURL_GLOBAL_ALL);

  // init the curl session */
  handle = curl_easy_init();

  // set URL to get here */
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

  // Allow single redirect
  curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

  /* Switch on full protocol/debug output while testing */
  //curl_easy_setopt(handle, CURLOPT_VERBOSE, verbose);

  /* disable progress meter, set to 0L to enable and disable debug output */
  curl_easy_setopt(handle, CURLOPT_NOPROGRESS, !verbose);

  /* send all data to this function  */
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data_file);
  // write to file
  outfile = std::fopen(out.c_str(), "wb");
  if (outfile) {
    // write page
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, outfile);
    // get page
    curl_easy_perform(handle);
    // close header file
    std::fclose(outfile);
  }

  // cleanup curl stuff
  curl_easy_cleanup(handle);
  curl_global_cleanup();

  return out;
}

// [[Rcpp::export]]
StringVector downloadthatcpp(const std::string& url,
                            const std::string& out) {
  return downloadthatcpp_(url, out);
}

// [[Rcpp::export]]
StringVector readthatcpp(const std::string& url) {
  return readthatcpp_(url);
}

// [[Rcpp::export]]
StringVector downloadthosecpp(const std::vector<std::string>& urls,
                              const std::vector<std::string>& outs) {
  const int n = urls.size();
  StringVector res(n);
  for (int i = 0; i < n; ++i) {
    res[i] = downloadthatcpp_(urls[i], outs[i]);
  }
  res.attr("names") = urls;
  return res;
}

// [[Rcpp::export]]
StringVector readthosecpp(const std::vector<std::string>& paths) {
  const int n = paths.size();
  StringVector res(n);
  for (int i = 0; i < n; ++i) {
    res[i] = readthatcpp_(paths[i]);
  }
  res.attr("names") = paths;
  return res;
}
