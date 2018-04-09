library(RProtoBuf)
readProtoFiles("src/proto/example.proto")
# ls("RProtoBuf:DescriptorPool")
# protoc -I "proto/" --cpp_out="proto/" "proto/example.proto"

x <- new(tensorflow.Feature, int64_list = new(tensorflow.Int64List, value = c(0:3)))
y <- new(tensorflow.Features.FeatureEntry, key = "my_ints", value = x)
z <- new(tensorflow.Features, feature = list(y))
w <- new(tensorflow.Example, features = z)

s <- w$serialize("myints_r.tfrecords")
system("gzip myints_r.tfrecords")

a <- raw_to_char(s)
writeLines(text = raw_to_char(s), "myints_r.tfrecords")

# \n\025\n\023\n\amy_ints\022\b\032\006\n\004\0\001\002\003
# \n\x14\n\x12\n\x07my_ints\x12\x07\x1a\x05\n\x03\x00\x01\x02

con <- RProtoBuf::FileOutputStream("my_ints.tfrecord", block_size = 1L, close.on.delete = TRUE)
con
RProtoBuf::WriteLittleEndian32(con, w$serialize(NULL))
