#' @keywords internal
"_PACKAGE"

# The following block is used by usethis to automatically manage
# roxygen namespace tags. Modify with care!
## usethis namespace: start
#' @useDynLib readthat, .registration = TRUE
#' @importFrom Rcpp sourceCpp
## usethis namespace: end
NULL


readweb <- function(url) {
  r <- httr::GET(url)
  rawToChar(r$content)
}

#' Read that file/webpage
#'
#' Returns text/source from file or URL
#'
#' @param .x Path to file or URL
#' @return A string of read-in text
#' @export
readthat <- function(.x) UseMethod("readthat")

#' @export
readthat.default <- function(.x) {
  readthatcpp(.x)
}
