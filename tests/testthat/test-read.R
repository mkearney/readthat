test_that("read functions works", {

  ##--------------------------------------------
  ## URL
  ##--------------------------------------------
  x <- read("https://mikewk.com")
  expect_gt(nchar(x), 100)
  expect_null(names(x))
  expect_length(x, 1L)

  ##--------------------------------------------
  ## path
  ##--------------------------------------------
  dir <- tempdir()
  tmp <- file.path(dir, "mikewk.com")
  writeLines(x, tmp)
  x <- read(tmp)
  expect_gt(nchar(x), 100)
  expect_null(names(x))
  expect_length(x, 1L)

  ##--------------------------------------------
  ## URL & path
  ##--------------------------------------------
  x <- sapply(c("https://mikewk.com", tmp), read)
  expect_equal(length(x), 2)
  expect_named(x)
  expect_equal(c("https://mikewk.com", tmp), names(x))
  expect_gt(nchar(x[1]), 100)
  expect_gt(nchar(x[2]), 100)


  ##--------------------------------------------
  ## errors
  ##--------------------------------------------
  expect_error(read(rnorm(10)))
  expect_error(read(c("https://mikewk.com", "https://mikewk.com")))
  expect_error(read(""))
  expect_error(read("https://"))
})
