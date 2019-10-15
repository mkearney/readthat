
<!-- README.md is generated from README.Rmd. Please edit that file -->

# readthat

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
x <- "~/Dropbox/data.txt"
bm_file <- bench::mark(
  readtext = readtext::readtext(x),
  readr = readr::read_lines(x),
  readthat = read(x),
  readLines = readLines(x),
  check = FALSE
)
bm_file
#> # A tibble: 4 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 readtext    754.5µs  781.3µs     1248.    4.88MB    12.5 
#> 2 readr       159.1µs  165.6µs     5865.     2.7MB     8.30
#> 3 readthat     40.7µs     42µs    23229.   23.94KB     6.97
#> 4 readLines    89.1µs   90.6µs    10710.    8.79KB     2.02
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
#> 1 httr           72ms  118.4ms      7.06    2.05MB    0    
#> 2 xml2        178.7ms  228.8ms      3.34    1.47MB    0.636
#> 3 readthat     51.2ms   64.4ms     12.3    24.01KB    0    
#> 4 readLines   335.5ms  390.7ms      2.38  446.78KB    0    
#> 5 readr       154.4ms  175.4ms      3.93  618.18KB    0
```

![](man/figures/README-bm_html.png)
