
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
#> 1 readtext    796.1µs  832.3µs     1180.    5.38MB    10.3 
#> 2 readr       156.8µs  162.6µs     5915.    2.69MB    10.4 
#> 3 readthat     27.7µs   28.9µs    33364.   21.69KB     0   
#> 4 readLines   132.2µs    142µs     6904.   10.54KB     2.01
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
#> 1 httr         77.8ms    221ms      5.36    2.81MB    0    
#> 2 xml2        211.7ms    252ms      3.84    1.88MB    0.960
#> 3 readthat    220.7ms    254ms      3.80   643.3KB    0    
#> 4 readLines   335.7ms    425ms      2.22  643.17KB    0    
#> 5 readr       157.8ms    189ms      2.72  845.47KB    0
ggplot2::autoplot(bm_html)
```

<img src="man/figures/README-unnamed-chunk-3-1.png" width="100%" />
