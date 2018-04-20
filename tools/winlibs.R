# Build against protobuf libs compiled with Rtools
# by Jeroen Ooms. Last updated: August 2016
if (!file.exists("../windows/protobuf-3.5.1/include/google/protobuf/descriptor.h")) {
  if (getRversion() < "3.3.0") setInternet2()
  download.file("https://github.com/rwinlib/protobuf/archive/v3.5.1.zip", "lib.zip", quiet = TRUE)
  dir.create("../windows", showWarnings = FALSE)
  unzip("lib.zip", exdir = "../windows")
  unlink("lib.zip")
  
  file.copy("../inst/proto/example.pb.cc", to = "example.pb.cc")
  file.copy("../inst/proto/example.pb.h", to = "example.pb.h")
  file.copy("../inst/proto/feature.pb.cc", to = "feature.pb.cc")
  file.copy("../inst/proto/feature.pb.h", to = "feature.pb.h")
  
}