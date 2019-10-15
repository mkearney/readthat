test_that("read functions works", {

  ##--------------------------------------------
  ## URL
  ##--------------------------------------------
  x <- readthat("https://mikewk.com")
  expect_gt(nchar(x), 100)
  expect_null(names(x))
  expect_length(x, 1L)

  ##--------------------------------------------
  ## path
  ##--------------------------------------------
  dir <- tempdir()
  tmp <- file.path(dir, "mikewk.com")
  writeLines(x, tmp)
  x <- readthat(tmp)
  expect_gt(nchar(x), 100)
  expect_null(names(x))
  expect_length(x, 1L)

  ##--------------------------------------------
  ## URL & path
  ##--------------------------------------------
  x <- sapply(c("https://mikewk.com", tmp), readthat)
  expect_equal(length(x), 2)
  expect_named(x)
  expect_equal(c("https://mikewk.com", tmp), names(x))
  expect_gt(nchar(x[1]), 100)
  expect_gt(nchar(x[2]), 100)


  ##--------------------------------------------
  ## errors
  ##--------------------------------------------
  expect_error(readthat(rnorm(10)))
  expect_error(readthat(c("https://mikewk.com", "https://mikewk.com")))
  expect_error(readthat(""))
  expect_error(readthat("https://"))
})
