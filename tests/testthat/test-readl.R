test_that("readl functions works", {

  ##--------------------------------------------
  ## URL
  ##--------------------------------------------
  x <- readl("https://mikewk.com")
  expect_true(is.character(x))
  expect_gt(length(x), 10)
  expect_gt(nchar(x[1]), 4)
  expect_null(names(x))

  ##--------------------------------------------
  ## path
  ##--------------------------------------------
  dir <- tempdir()
  tmp <- file.path(dir, "mikewk.com")
  writeLines(x, tmp)
  x <- readl(tmp)
  expect_true(is.character(x))
  expect_gt(length(x), 10)
  expect_gt(nchar(x[1]), 4)
  expect_null(names(x))

  ##--------------------------------------------
  ## first and last
  ##--------------------------------------------
  x <- readl(tmp, 1, 10)
  expect_equal(length(x), 10)
  expect_gt(nchar(x[1]), 4)
  expect_null(names(x))
  x <- readl(tmp, 10, 11)
  expect_true(is.character(x))
  expect_equal(length(x), 2)
  expect_null(names(x))

  ##--------------------------------------------
  ## errors
  ##--------------------------------------------
  expect_error(readl(rnorm(10)))
  expect_error(readl(c("https://mikewk.com", "https://mikewk.com")))
  expect_error(readl(""))
  expect_error(readl("https://"))

  ##--------------------------------------------
  ## cleanup
  ##--------------------------------------------
  if (file.exists(tmp)) {
    unlink(tmp)
  }
})
