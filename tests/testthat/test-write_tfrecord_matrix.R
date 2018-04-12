context("write_tfrecord_matrix")

library(tensorflow)
library(tfdatasets)

parse_function <- function(example_proto) {
  features = dict("x" = tf$FixedLenFeature(shape(10), tf$int64))
  tf$parse_single_example(example_proto, features)
}

if (fs::file_exists("example.tfrecords"))
  fs::file_delete("example.tfrecords")

test_that("writing tfrecord matrix works", {
  
  x <- matrix(1:1000, nrow = 100, ncol = 10)
  tfrecords:::write_tfrecord(x, "example.tfrecords")
  
  df <- tfrecord_dataset("example.tfrecords")
  df <- df %>% dataset_map(parse_function)
  df <- df %>% dataset_batch(100)
  batch <- next_batch(df)
  sess <- tf$Session()
  x_recovered <- sess$run(batch)$x
  
  expect_equal(x, x_recovered)
  
})


test_that("writing a list of matrix", {
  
  x <- matrix(1:1000, nrow = 100, ncol = 10)
  y <- matrix(runif(1000), nrow = 100, ncol = 10)
  
  tfrecords:::write_tfrecords(list(x = x, y = y), "examplelist.tfrecords")
  
  parse_function <- function(example_proto) {
    features = dict(
      "x" = tf$FixedLenFeature(shape(10), tf$int64),
      "y" = tf$FixedLenFeature(shape(10), tf$float32)
      )
    tf$parse_single_example(example_proto, features)
  }
  
  df <- tfrecord_dataset("examplelist.tfrecords")
  df <- df %>% dataset_map(parse_function)
  df <- df %>% dataset_batch(100)
  batch <- next_batch(df)
  sess <- tf$Session()
  x_recovered <- sess$run(batch)
  
  
})
