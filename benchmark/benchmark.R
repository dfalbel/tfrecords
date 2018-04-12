library(tensorflow)

x <- matrix(1:1000, nrow = 100, ncol = 10)

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

microbenchmark::microbenchmark(
  tfrecords = tfrecords:::write_tfrecord(x, "example.tfrecords"),
  tensorflow = write_tfrecords_tf(x, "example2.tfrecords"), 
  times = 5
)