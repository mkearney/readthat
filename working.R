url <- "https://mikewk.com/post/2019-04-23-three-things-to-know-beyond-base-r/index.html"
path <- "~/Dropbox/data.txt"

x <- path
bm_file <- bench::mark(
  readthat = readthatcpp(x),
  readLines = readLines(x),
  readtext = readtext::readtext(x),
  readr = readr::read_lines(x),
  check = FALSE
)


x <- url
bm_html <- bench::mark(
  readthat = readthatcpp(x),
  readLines = readLines(x),
  readtext = readtext::readtext(x),
  readr = readr::read_lines(x),
  check = FALSE
)

substr(head(readthatcpp(path)), 1, 100)
substr(head(readthatcpp(url)), 1, 100)
substr(head(readtext::readtext(url)), 1, 100)

library(lop)
d <- tbl(
  readr = paste0(paste(readr::read_lines(x), collapse = "\n"), "\n"),
  readthat = readthatcpp(x)
)

d
identical(d$readr, d$readthat)

list(d$readthat, d$readr) %>%
  tfse::substrev(50)
  lap(nchar)



ggplot2::autoplot(bm_file) +
  ggplot2::ggsave("man/figures/README-bm_file.png")
ggplot2::autoplot(bm_html) +
  ggplot2::ggsave("man/figures/README-bm_html.png")
dir.create("man/figures")
ggplot2::autoplot(bm_html)
bm_file
bm_html
