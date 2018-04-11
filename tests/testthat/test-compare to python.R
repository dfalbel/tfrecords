context("compare to python")

ref_file <- paste0(rprojroot::find_testthat_root_file(), "/files/myints_py.tfrecords")

test_that("writing correctly compared to python", {
  
  #fs::file_delete("example2.tfrecords")
  tfrecords:::DummyExample()
  
  ref_file <- readr::read_file_raw(ref_file)
  gen_file <- readr::read_file_raw("example2.tfrecords")
  
  
  
  l_ref <- readBin(ref_file[1:8], what = "integer")
  l_gen <- readBin(gen_file[1:8], what = "integer")
  
  expect_equal(l_ref, l_gen)
  

})
