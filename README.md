
<!-- README.md is generated from README.Rmd. Please edit that file -->

# readthat

<!-- badges: start -->

[![CRAN
status](https://www.r-pkg.org/badges/version/readthat)](https://CRAN.R-project.org/package=readthat)
[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://www.tidyverse.org/lifecycle/#experimental)
<!-- badges: end -->

Quickly read text/source from local files and web pages.

## Installation

You can install the development released version of readthat from Github
with:

``` r
remotes::install_github("mkearney/readthat")
```

## Comparisons

Benchmark comparison for reading a text file:

``` r
x <- "~/Dropbox/data.txt"
bm_file <- bench::mark(
  readtext = readtext::readtext(x),
  readr = readr::read_lines(x),
  readthat = readthat(x),
  readLines = readLines(x),
  check = FALSE
)
bm_file
#> # A tibble: 4 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 readtext    799.6µs  829.9µs     1200.    5.38MB    10.3 
#> 2 readr       159.1µs  163.7µs     5989.    2.69MB    10.4 
#> 3 readthat     32.4µs   33.1µs    29476.    21.7KB     0   
#> 4 readLines   131.3µs  133.1µs     7377.   10.54KB     2.01
ggplot2::autoplot(bm_file)
#> Loading required namespace: tidyr
```

<img src="man/figures/README-unnamed-chunk-2-1.png" width="100%" />

Benchmark comparison for reading a web page:

``` r
x <- "https://www.espn.com/nfl/scoreboard"
bm_html <- bench::mark(
  httr = httr::content(httr::GET(x), as = "text", encoding = "UTF-8"),
  xml2 = xml2::read_html(x),
  readthat = readthat(x),
  readLines = readLines(x, warn = FALSE),
  readr = readr::read_lines(x),
  check = FALSE,
  iterations = 10
)
bm_html
#> # A tibble: 5 x 6
#>   expression      min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr> <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 httr          132ms    273ms      3.62    2.81MB    0    
#> 2 xml2          233ms    270ms      3.64    1.89MB    0.910
#> 3 readthat      251ms    287ms      3.43  645.28KB    0    
#> 4 readLines     380ms    448ms      2.05  646.43KB    0    
#> 5 readr         154ms    249ms      3.99  843.72KB    0
ggplot2::autoplot(bm_html)
```

<img src="man/figures/README-unnamed-chunk-3-1.png" width="100%" />
