context("write_tfrecord_matrix")

test_that("writing a list of matrix", {
  
  testthat::skip_if_not(reticulate::py_module_available("tensorflow"))
  
  library(tensorflow)
  library(tfdatasets)
  library(reticulate)
  
  os <- import("os")
  x <- os$environ$setdefault('TF_CPP_MIN_LOG_LEVEL', '3')
  
  temp <- tempfile()
  
  x <- matrix(1:1000, nrow = 100, ncol = 10)
  y <- matrix(runif(1000), nrow = 100, ncol = 10)
  
  tfrecords::write_tfrecords(list(x = x, y = y), temp)
  
  parse_function <- function(example_proto) {
    features = dict(
      "x" = tf$FixedLenFeature(shape(10), tf$int64),
      "y" = tf$FixedLenFeature(shape(10), tf$float32)
      )
    tf$parse_single_example(example_proto, features)
  }
  
  suppressWarnings(suppressMessages(df <- tfrecord_dataset(temp)))
  df <- df %>% dataset_map(parse_function)
  df <- df %>% dataset_batch(100)
  batch <- next_batch(df)
  sess <- tf$Session()
  x_recovered <- sess$run(batch)
  
  expect_equal(x, x_recovered$x)
  expect_equivalent(y, x_recovered$y, tolerance = 2e-7) # it can be different because of floating points convertion
})



test_that("support for sparse matrix", {
  
  testthat::skip_if_not(reticulate::py_module_available("tensorflow"))
  
  temp <- tempfile()
  
  set.seed(1)
  
  data <- list(
    x = matrix(1:1000, nrow = 100),
    y = matrix(1:1000/2.1, nrow = 100),
    z = Matrix::rsparsematrix(nrow = 100, ncol = 10, density = 0.3)
    )
  
  write_tfrecords(data, temp)
  
  parse_function <- function(example_proto) {
    features = dict(
      "x" = tf$FixedLenFeature(shape(10), tf$int64),
      "y" = tf$FixedLenFeature(shape(10), tf$float32),
      "z" = tf$SparseFeature(index_key = "index_z", value_key = "value_z", dtype = tf$float32, size = 10L)
    )
    features <- tf$parse_single_example(example_proto, features)
    features$z <- tf$sparse_tensor_to_dense(features$z)
    features
  }
  
  df <- tfrecord_dataset(temp)
  df <- df %>% dataset_map(parse_function)
  df <- df %>% dataset_batch(100)
  batch <- next_batch(df)
  sess <- tf$Session()
  x_recovered <- sess$run(batch)
  
  expect_equal(data$x, x_recovered$x)
  expect_equivalent(data$y, x_recovered$y, tolerance = 2e-7) # it can be different because of floating points convertion
  expect_equivalent(as.matrix(data$z), x_recovered$z, tolerance = 2e-7) # it can be different because of floating points convertion
})


