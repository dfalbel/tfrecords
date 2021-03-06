library(tensorflow)

x <- matrix(1:1000000, nrow = 100000)

write_tfrecords_tf <- function(x, path) {
  
  writer <- tf$python_io$TFRecordWriter(path)
  
  for (i in 1:nrow(x)) {
    
    example <- tf$train$Example(features=tf$train$Features(feature=dict(
      'x'= tf$train$Feature(int64_list=tf$train$Int64List(value=x[i,]))
    )))
    
    writer$write(example$SerializeToString())
    
  }
  
  writer$close()
  
}

tf$VERSION # finish loading TF

microbenchmark::microbenchmark(
  tfrecords = tfrecords::write_tfrecords(list(x = x), "example.tfrecords"),
  tensorflow = write_tfrecords_tf(x, "example2.tfrecords"), 
  times = 5
)

identical(
  readr::read_file_raw("example.tfrecords"),
  readr::read_file_raw("example2.tfrecords")
)

