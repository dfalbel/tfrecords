context("write_tfrecord_array")

library(tensorflow)
library(tfdatasets)
library(reticulate)

os <- import("os")
x <- os$environ$setdefault('TF_CPP_MIN_LOG_LEVEL', '3')

temp <- "test.tfrecords"#tempfile()

test_that("writing a list of arrays", {
  
  x <- matrix(1:1000, nrow = 100, ncol = 10)
  y <- array(1:3000, dim = c(100, 5, 2, 3))
  z <- array(1:1000, dim = c(100, 2, 5))
  
  tfrecords::write_tfrecords(list(x = x, y = y, z = z), temp)
  
  parse_function <- function(example_proto) {
    features = dict(
      "x" = tf$FixedLenFeature(shape(10), tf$int64),
      "y" = tf$FixedLenFeature(shape(5, 2, 3), tf$int64),
      "z" = tf$FixedLenFeature(shape(2, 5), tf$int64)
      )
    tf$parse_single_example(example_proto, features)
  }
  
  df <- tfrecord_dataset(temp)
  df <- df %>% dataset_map(parse_function)
  df <- df %>% dataset_batch(100)
  batch <- next_batch(df)
  sess <- tf$Session()
  x_recovered <- sess$run(batch)
  
  expect_equal(x, x_recovered$x)
  expect_equivalent(y, x_recovered$y) 
  expect_equal(z, x_recovered$z)
})

