% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/read.R
\name{read}
\alias{read}
\title{Read file/webpage}
\usage{
read(.x)
}
\arguments{
\item{.x}{Path to file or URL}
}
\value{
A string of read-in text
}
\description{
Returns text/source from file or URL
}
\examples{

## create a temp file containing multiple lines of text
tmp <- tempfile()
cat("TITLE", "", "By", "", "Some more text here", "",
  "last line", sep = "\\n", file = tmp)

## read contents from tmp
read(tmp)

## cleanup
unlink(tmp)

## read contents from a website
read("http://httpbin.org/get")

}
\seealso{
Other read: \code{\link{readl}}
}
\concept{read}
