#' Download file(s)/webpage(s)
#'
#' Downloads text/source from multiple files or URLs
#'
#' @param .x Paths to files or URLs
#' @param .o File names to save the texts/sources as.
#' @return A character vector where each element is a string of read-in text
#'   associated with each input.
#' @export
downloadthose <- function(.x, .o) UseMethod("downloadthose")

#' @export
downloadthose.default <- function(.x, .o) {
  downloadthosecpp(.x, .o)
}
