

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
  if (has_fs()) {
    readthatcpp(.x)
  } else {
    readthatcpp2(.x)
  }
}


has_fs <- function() {
  v <- paste(system("c++ --version", intern = TRUE), collapse = "\n")
  !grepl("Apple LLVM version (10|[1-9])\\.", v)
}
