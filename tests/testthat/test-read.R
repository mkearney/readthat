test_that("read functions works", {

  ##--------------------------------------------
  ## THAT SINGLE
  ##--------------------------------------------
  x <- readthat("https://mikewk.com")
  expect_gt(nchar(x), 100)
  expect_named(x)

  x <- readthat2("https://mikewk.com")
  expect_gt(nchar(x), 100)
  expect_named(x)

  dir <- tempdir()
  tmp <- file.path(dir, "mikewk.com")
  x <- downloadthat("https://mikewk.com", tmp)
  expect_true(is.character(x))
  expect_equal(tmp, x)

  ##--------------------------------------------
  ## THOSE MULTIPLE
  ##--------------------------------------------
  x <- readthose(c("https://mikewk.com", tmp))
  expect_equal(length(x), 2)
  expect_gt(nchar(x[1]), 100)
  expect_gt(nchar(x[2]), 100)
  expect_named(x)

  x <- readthose2(c("https://mikewk.com", tmp))
  expect_equal(length(x), 2)
  expect_gt(nchar(x[1]), 100)
  expect_gt(nchar(x[2]), 100)
  expect_named(x)

  x <- downloadthose(c("https://mikewk.com", "https://cv.mikewk.com"),
    c(file.path(dir, "test1"), file.path(dir, "test2")))
  expect_true(file.exists(file.path(dir, "test1")))
  expect_true(file.exists(file.path(dir, "test2")))
})
