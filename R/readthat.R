

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
