#' Read file/webpage
#'
#' Returns text/source from file or URL
#'
#' @param .x Path to file or URL
#' @return A string of read-in text
#' @export
#' @family read
readthat <- function(.x) UseMethod("readthat")

#' @export
readthat.default <- function(.x) {
  readthatcpp(.x)
}

#' Read file(s)/webpage(s)
#'
#' Returns text/source from multiple files or URLs
#'
#' @param .x Paths to files or URLs
#' @return A character vector where each element is a string of read-in text
#'   associated with each input.
#' @export
#' @family read
readthose <- function(.x) UseMethod("readthose")

#' @export
readthose.default <- function(.x) {
  readthosecpp(.x)
}
