library(tensorflow)

my_example = tf$train$Example(features=tf$train$Features(feature=dict(
  'my_ints'= tf$train$Feature(int64_list=tf$train$Int64List(value=c(0L, 1L, 2L)))
)))

my_example_str = my_example$SerializeToString()

writer = tf$python_io$TFRecordWriter("myints_py.tfrecords")
writer$write(my_example_str)
writer$close()

df <- tfdatasets::tfrecord_dataset("myints_py.tfrecords")
