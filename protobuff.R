library(RProtoBuf)
readProtoFiles("proto/example.proto")
# ls("RProtoBuf:DescriptorPool")
# protoc -I "proto/" --cpp_out="proto/" "proto/example.proto"

x <- new(tensorflow.Feature, int64_list = new(tensorflow.Int64List, value = c(0:3)))
y <- new(tensorflow.Features.FeatureEntry, key = "my_ints", value = x)
z <- new(tensorflow.Features, feature = list(y))
w <- new(tensorflow.Example, features = z)

s <- w$serialize(NULL)
writeLines(text = rawToChar(s), "myints_r.tfrecords")

df <- tfdatasets::tfrecord_dataset("myints_r.tfrecords")

# \n\025\n\023\n\amy_ints\022\b\032\006\n\004\0\001\002\003
# \n\x14\n\x12\n\x07my_ints\x12\x07\x1a\x05\n\x03\x00\x01\x02

