library(RProtoBuf)
readProtoFiles("proto/example.proto")
# ls("RProtoBuf:DescriptorPool")


x <- new(tensorflow.Feature, int64_list = new(tensorflow.Int64List, value = c(0:3)))
y <- new(tensorflow.Features.FeatureEntry, key = "my_ints", value = x)
z <- new(tensorflow.Features, feature = list(y))
w <- new(tensorflow.Example, features = z)

serialize(w, "myints_r.tfrecords")

df <- tfdatasets::tfrecord_dataset("myints_r.tfrecords")


