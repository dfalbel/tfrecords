// [[Rcpp::depends(BH)]]
#include "example.pb.h" // needs to be included first because of the Free macro
#include <Rcpp.h>
#include <fstream>
#include <stddef.h>
#include "RecordWriter.h"
#include "Example.h"

using namespace Rcpp;

// [[Rcpp::export]]
bool write_tfrecord(Rcpp::IntegerMatrix x, std::string path) {
  
  Example example;
  RecordWriter writer(path);
  
  for (int i=0; i<x.nrow(); i++) {
    
    example.set_int_var("x", x(i,_));
    writer.write_record(example.serialize_to_string());
    example.clear();
    
  }
  
  return true;
} 

// [[Rcpp::export]]
bool tfrecord_shutdown() {
  google::protobuf::ShutdownProtobufLibrary();
  return true;
}

// [[Rcpp::export]]
std::string write_test_example (std::string path) {
  
  // Create Example
  tensorflow::Example example;
  auto * features = example.mutable_features();
  auto feature = features->mutable_feature();
  
  tensorflow::Feature new_feat;
  auto new_feat_ints = new_feat.mutable_int64_list();
  new_feat_ints->add_value(0);
  new_feat_ints->add_value(1);
  new_feat_ints->add_value(2);
  
  using pair_type = google::protobuf::MapPair<std::string, tensorflow::Feature>;
  feature->insert(pair_type("my_ints", new_feat));
  
  std::string out;
  example.SerializeToString(&out);
  
  RecordWriter rec_writer(path);
  rec_writer.write_record(out);
  
  return out;
}
