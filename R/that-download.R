
#' Download file/webpage
#'
#' Downloads text/source from file or URL
#'
#' @param .x Path to file or URL
#' @param .o File name to save the text/source as.
#' @return A string of read-in text
#' @export
downloadthat <- function(.x, .o) UseMethod("downloadthat")

#' @export
downloadthat.default <- function(.x, .o) {
  downloadthatcpp(.x, .o)
}
