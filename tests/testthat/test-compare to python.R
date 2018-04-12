context("compare to python")

ref_file <- paste0(rprojroot::find_testthat_root_file(), "/files/myints_py.tfrecords")

if (fs::file_exists("example2.tfrecords")) 
  fs::file_delete("example2.tfrecords")
tfrecords:::write_test_example("example2.tfrecords")

test_that("writing correctly compared to python", {
  
  ref_file <- readr::read_file_raw(ref_file)
  gen_file <- readr::read_file_raw("example2.tfrecords")
  
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
