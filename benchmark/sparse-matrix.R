rows <- 1000
cols <- 100

x <- Matrix::rsparsematrix(nrow = rows, ncol = cols, density = 0.3)

temp <- tempfile(fileext = "tfrecords")

microbenchmark::microbenchmark(
  sp = tfrecords::write_tfrecords(list(x = x), path = temp)  
)


# Unit: milliseconds                                                                                                           
# expr      min       lq     mean   median       uq      max neval
# sp 23.97385 26.89855 29.53111 28.60625 30.87249 60.84984   100