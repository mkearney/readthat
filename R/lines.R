#' Read \bold{lines} from file/webpage
#'
#' Returns \bold{lines} of text/source from file or URL
#'
#' @param .x Path to file or URL
#' @param first The first (starting) line number in which reading should start (inclusive). Defaults to 1.
#' @param last The last (ending) line number in which reading should end (inclusive). To ensure the reading of all possible lines, set this value to less than 1 (0 or -1). Defaults to -1.
#' @return A vector of lines read from input
#' @export
#' @family read
#' @examples
#'
#' ## create a temp file containing multiple lines of text
#' tmp <- tempfile()
#' cat("TITLE", "", "By", "", "Some more text here", "",
#'   "last line", sep = "\n", file = tmp)
#'
#' ## read all lines of tmp
#' readl(tmp)
#'
#' ## read first three lines
#' readl(tmp, 1, 3)
#'
#' ## read lines 3-5
#' readl(tmp, 3, 5)
#'
#' ## cleanup
#' unlink(tmp)
#'
readl <- function(.x, first = 1, last = -1) UseMethod("readl")

#' @export
readl.default <- function(.x, first = 1, last = -1) {
  stopifnot(
    length(.x) == 1,
    is.character(.x),
    is_num(first),
    is_num(last)
  )
  if (grepl("^http", .x)) {
    return(readl_url(.x, first, last))
  }
  readl_path(.x, first, last)
}

readl_path <- function(.x, first = 1, last = -1) {
  stopifnot(file.exists(.x))
  readlcpp(normalizePath(.x), first, last)
}

readl_url <- function(.x, first = 1, last = -1) {
  h <- curl::new_handle(followlocation = 1)
  curl::handle_setheaders(h,
    `User-Agent` = 'readthat',
    `Accept` = 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
    `Accept-Encoding` = 'gzip, deflate, br',
    `Content-Type` = "text/html; charset=utf-8",
    `Cache-Control` = "max-age=600"
  )
  r <- curl::curl_fetch_disk(.x, tempfile(), h)
  readlcpp(r$content, first, last)
}

is_num <- function(x) {
  if (is.numeric(x)) {
    return(TRUE)
  }
  x <- suppressWarnings(
    as.numeric(x)
  )
  all(!is.na(x))
}
