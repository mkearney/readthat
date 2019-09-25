#' Read file(s)/webpage(s)
#'
#' Returns text/source from multiple files or URLs
#'
#' @param .x Paths to files or URLs
#' @return A character vector where each element is a string of read-in text
#'   associated with each input.
#' @export
readthose <- function(.x) UseMethod("readthose")

#' @export
readthose.default <- function(.x) {
  readthosecpp(.x)
}
