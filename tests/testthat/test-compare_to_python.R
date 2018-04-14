context("compare to python")

# creating reference file -------------------------------------------------
library(tensorflow)

my_example = tf$train$Example(features=tf$train$Features(feature=dict(
  'my_ints'= tf$train$Feature(int64_list=tf$train$Int64List(value=c(0L, 1L, 2L)))
)))

my_example_str = my_example$SerializeToString()

writer = tf$python_io$TFRecordWriter("example_py.tfrecords")
writer$write(my_example_str)
writer$close()


# write test file ---------------------------------------------------------

tfrecords:::write_test_example("example_r.tfrecords")


# testing -----------------------------------------------------------------

ref_file <- "example_py.tfrecords"
gen_file <- "example_r.tfrecords"

test_that("writing correctly compared to python", {
  
  ref_file <- readr::read_file_raw(ref_file)
  gen_file <- readr::read_file_raw(gen_file)
  
  l_ref <- readBin(ref_file[1:8], what = "integer")
  l_gen <- readBin(gen_file[1:8], what = "integer")
  
  l_crc_ref <- readBin(ref_file[9:12], what = "integer")
  l_crc_gen <- readBin(gen_file[9:12], what = "integer")
  
  data_ref <- readBin(ref_file[13:(13 + l_ref-1)], what = "character")
  data_gen <- readBin(gen_file[13:(13 + l_gen-1)], what = "character")
  
  data_crc_ref <- readBin(tail(ref_file, 4), what = "integer")
  data_crc_gen <- readBin(tail(gen_file, 4), what = "integer")
  
  expect_equal(l_ref, l_gen)
  expect_equal(l_crc_ref, l_crc_gen)
  expect_equal(data_ref, data_gen)
  expect_equal(data_crc_ref, data_crc_gen)
  expect_equal(ref_file, gen_file)

})