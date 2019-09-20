url <- "https://mikewk.com/post/2019-04-23-three-things-to-know-beyond-base-r/index.html"
path <- "~/Dropbox/data.txt"

x <- path
bm_file <- bench::mark(
  readit = readitcpp(x),
  readLines = readLines(x),
  readtext = readtext::readtext(x),
  readr = readr::read_lines(x),
  check = FALSE
)


x <- url
bm_html <- bench::mark(
  readit = readitcpp(x),
  readLines = readLines(x),
  readtext = readtext::readtext(x),
  readr = readr::read_lines(x),
  check = FALSE
)

substr(head(readitcpp(path)), 1, 100)
substr(head(readitcpp(url)), 1, 100)
substr(head(readtext::readtext(url)), 1, 100)

library(lop)
d <- tbl(
  readr = paste0(paste(readr::read_lines(x), collapse = "\n"), "\n"),
  readit = readitcpp(x)
)

d
identical(d$readr, d$readit)

list(d$readit, d$readr) %>%
  tfse::substrev(50)
  lap(nchar)



ggplot2::autoplot(bm_file)
ggplot2::autoplot(bm_html)
bm_file
bm_html
