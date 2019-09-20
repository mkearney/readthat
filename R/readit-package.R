#' @keywords internal
"_PACKAGE"

# The following block is used by usethis to automatically manage
# roxygen namespace tags. Modify with care!
## usethis namespace: start
#' @useDynLib readit, .registration = TRUE
#' @importFrom Rcpp sourceCpp
## usethis namespace: end
NULL


readweb <- function(url) {
  r <- httr::GET(url)
  rawToChar(r$content)
}
