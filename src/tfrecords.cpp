// [[Rcpp::depends(BH)]]
#include "example.pb.h" // needs to be included first because of the Free macro
#include <Rcpp.h>
#include <fstream>
#include <stddef.h>
#include "crc.hpp"

using namespace Rcpp;

// This is a simple example of exporting a C++ function to R. You can
// source this function into an R session using the Rcpp::sourceCpp 
// function (or via the Source button on the editor toolbar). Learn
// more about Rcpp at:
//
//   http://www.rcpp.org/
//   http://adv-r.had.co.nz/Rcpp.html
//   http://gallery.rcpp.org/
//x

// [[Rcpp::export]]
std::string DummyExample () {
  
  // Create Example
  tensorflow::Example example;
  auto * features = example.mutable_features();
  auto feature = features->mutable_feature();
  
  tensorflow::Feature new_feat;
  auto new_feat_ints = new_feat.mutable_int64_list();
  new_feat_ints->add_value(1);
  new_feat_ints->add_value(2);
  
  using pair_type = google::protobuf::MapPair<std::string, tensorflow::Feature>;
  feature->insert(pair_type("key", new_feat));
  
  std::string out;
  example.SerializeToString(&out);
  
  std::ofstream myfile;
  myfile.open ("example.tfrecords");
  myfile << out << std::endl;
  myfile.close();
  
  Rcout << out << "\n";
  
  return out;
}

// helper functions

// mask delta constant
// https://github.com/tensorflow/tensorflow/blob/754048a0453a04a761e112ae5d99c149eb9910dd/tensorflow/core/lib/hash/crc32c.h#L33
const uint32_t kMaskDelta = 0xa282ead8ul;

// making crc
// https://github.com/tensorflow/tensorflow/blob/754048a0453a04a761e112ae5d99c149eb9910dd/tensorflow/core/lib/hash/crc32c.h#L40
uint32_t mask(uint32_t crc) {
  return ((crc >> 15) | (crc << 17)) + kMaskDelta;
};

// Get CRC32 rep
// https://stackoverflow.com/a/39381287/3297472
int crc32(const std::string& my_string) {
  boost::crc_32_type result;
  result.process_bytes(my_string.data(), my_string.length());
  return result.checksum();
};

class RecordWriter {
public: 
  
  std::string path;
  
  RecordWriter (std::string path) {
    this->path = path;
  }
  
  bool write_record (std::string record) {
    
    // Format of a single record:
    //  uint64    length
    //  uint32    masked crc of length
    //  byte      data[length]
    //  uint32    masked crc of data
    
    auto length = sizeof(record);
    std::uint64_t length_uiint64 = length;
    
    
    return true;
  }
  
  
};


// [[Rcpp::export]]
Rcpp::LogicalVector tfrecord_shutdown() {
  google::protobuf::ShutdownProtobufLibrary();
  return 1;
}

