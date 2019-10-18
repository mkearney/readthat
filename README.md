
<!-- README.md is generated from README.Rmd. Please edit that file -->

# readthat <img src='man/figures/logo.png' align="right" height="173.5" />

<!-- badges: start -->

[![CRAN
status](https://www.r-pkg.org/badges/version/readthat)](https://CRAN.R-project.org/package=readthat)
[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)
[![Travis build
status](https://travis-ci.org/mkearney/readthat.svg?branch=master)](https://travis-ci.org/mkearney/readthat)
[![Codecov test
coverage](https://codecov.io/gh/mkearney/readthat/branch/master/graph/badge.svg)](https://codecov.io/gh/mkearney/readthat?branch=master)
<!-- badges: end -->

Quickly read text/source from local files and web pages.

## Installation

You can install the development released version of readthat from Github
with:

``` r
remotes::install_github("mkearney/readthat")
```

## Examples

Let’s say we want to read-in the source of the following websites:

``` r
## a vector of URLs
urls <- c(
  "https://mikewk.com",
  "https://cnn.com",
  "https://www.cnn.com/us"
)
```

Use `readthat::read()` to read the text/source of a single file/URL

``` r
## read single web/file (returns text vector)
x <- read(urls[1])

## preview output
substr(x, 1, 60)
#> [1] "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\n  <meta charset=\"ut"

## use apply functions to read multiple pages
xx <- sapply(urls, read)

## preview output
lapply(xx, substr, 1, 60)
#> $`https://mikewk.com`
#> [1] "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n\n  <meta charset=\"ut"
#> 
#> $`https://cnn.com`
#> [1] "<!DOCTYPE html><html class=\"no-js\"><head><meta content=\"IE=e"
#> 
#> $`https://www.cnn.com/us`
#> [1] "<!DOCTYPE html><html class=\"no-js\"><head><meta content=\"IE=e"
```

## Comparisons

Benchmark comparison for reading a text file:

``` r
## save a text file
writeLines(read(urls[1]), x <- tempfile())

## coompare read times
bm_file <- bench::mark(
  readr = readr::read_lines(x),
  readthat = read(x),
  readLines = readLines(x),
  check = FALSE
)

## view results
bm_file
#> # A tibble: 3 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 readr       262.2µs  273.9µs     3517.    3.69MB     4.05
#> 2 readthat     83.7µs   86.2µs    10741.   28.65KB     4.04
#> 3 readLines   144.8µs  150.7µs     6311.   13.16KB     0
```

![](man/figures/README-bm_file.png)

Benchmark comparison for reading a web page:

``` r
x <- "https://www.espn.com/nfl/scoreboard"
bm_html <- bench::mark(
  httr = httr::content(httr::GET(x), as = "text", encoding = "UTF-8"),
  xml2 = xml2::read_html(x),
  readthat = read(x),
  readLines = readLines(x, warn = FALSE),
  readr = readr::read_lines(x),
  check = FALSE,
  iterations = 25,
  filter_gc = TRUE
)
bm_html
#> # A tibble: 5 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 httr         71.5ms    104ms      6.84     2.7MB    0.285
#> 2 xml2        187.9ms    200ms      4.50    1.85MB    1.42 
#> 3 readthat     48.6ms     52ms     14.1     23.9KB    0    
#> 4 readLines   375.9ms    472ms      1.95  620.33KB    0    
#> 5 readr       158.6ms    169ms      5.57   799.7KB    0.232
```

![](man/figures/README-bm_html.png)
