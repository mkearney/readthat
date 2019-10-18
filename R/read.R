#' Read file/webpage
#'
#' Returns text/source from file or URL
#'
#' @param .x Path to file or URL
#' @return A string of read-in text
#' @export
#' @family read
#' @examples
#'
#' ## create a temp file containing multiple lines of text
#' tmp <- tempfile()
#' cat("TITLE", "", "By", "", "Some more text here", "",
#'   "last line", sep = "\n", file = tmp)
#'
#' ## read contents from tmp
#' read(tmp)
#'
#' ## cleanup
#' unlink(tmp)
#'
#' ## read contents from a website
#' read("http://httpbin.org/get")
#'
read <- function(.x) UseMethod("read")

#' @export
read.default <- function(.x) {
  stopifnot(
    length(.x) == 1,
    is.character(.x)
  )
  if (grepl("^http", .x)) {
    return(read_url(.x))
  }
  read_path(.x)
}

read_path <- function(.x) {
  stopifnot(file.exists(.x))
  readcpp(normalizePath(.x))
}

read_url <- function(.x) {
  h <- curl::new_handle(followlocation = 1)
  curl::handle_setheaders(h,
    `User-Agent` = 'readthat',
    `Accept` = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
    `Accept-Encoding` = 'gzip, deflate, br',
    `Content-Type` = "text/html; charset=utf-8",
    `Cache-Control` = "max-age=600"
  )
  r <- curl::curl_fetch_memory(.x, h)
  rawToChar(r$content)
}
