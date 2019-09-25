#include <Rcpp.h>
#include <curl/curl.h>
#include <sstream>
#include <iostream>

using namespace Rcpp;

static std::size_t write_data_string(void *ptr,
                                     std::size_t size,
                                     std::size_t nmemb,
                                     void *stream) {
  std::string data((const char*) ptr, (std::size_t) size * nmemb);
  *((std::stringstream*) stream) << data << std::endl;
  return size * nmemb;
}


// [[Rcpp::export]]
std::string readthiscpp(std::string url) {
  curl_global_init(CURL_GLOBAL_ALL);

  CURL *handle = curl_easy_init();
  if(handle == NULL) {
    fprintf(stderr, "curl_easy_init() failed\n");
  }
  CURLcode code = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
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

static std::size_t write_data_file(void *ptr,
                                   std::size_t size,
                                   std::size_t nmemb,
                                   void *stream) {
  std::size_t written = std::fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

// [[Rcpp::export]]
std::string downloadthatcpp(const std::string& url,
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
