urls <- c(
  "https://www.espn.com/mlb/scoreboard",
  "https://www.cnn.com",
  "https://www.cnn.com/world",
  "https://www.cnn.com/us",
  "https://mikewk.com"
)

outs <- dapr::vap_chr(urls, ~ tempfile())

readthat(urls[5])
downloadthat(urls[5], "/tmp/mikewkcom.html")
x <- readthose(urls)
str(x, 1)

o <- dapr::vap_int(x, nchar)


x <- readthosecpp(urls)
substr(x, 1, 100)

m <- bench::mark(
  asyncpp <- asyncpp(urls),
  readthemcpp = readthosecpp(urls, outs),
  check = FALSE,
  iterations = 2
)
m
ggplot2::autoplot(m)


