
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
#> 1 readtext    810.2µs    838µs     1175.    4.89MB    10.3 
#> 2 readr       162.2µs    167µs     5794.     2.7MB    10.4 
#> 3 readthat     40.8µs     42µs    23061.   23.94KB     4.61
#> 4 readLines   131.5µs    133µs     7214.   10.54KB     2.01
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
#> 1 httr         78.7ms  135.5ms      6.99    2.08MB    0.291
#> 2 xml2          175ms  205.4ms      4.60    1.49MB    0.876
#> 3 readthat     48.8ms   54.2ms     14.5    23.99KB    0    
#> 4 readLines   311.2ms  322.3ms      2.83  455.72KB    0    
#> 5 readr       148.8ms  172.6ms      4.03  626.36KB    0.168
```

![](man/figures/README-bm_html.png)
