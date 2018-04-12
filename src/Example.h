#include "example.pb.h" // needs to be included first because of the Free macro
#include <Rcpp.h>
using namespace Rcpp;

using pair_type = google::protobuf::MapPair<std::string, tensorflow::Feature>;

class Example {
  
public:
  
  tensorflow::Example example;
  tensorflow::Features * features;
  google::protobuf::Map<std::string, tensorflow::Feature> * feature;
  
  Example () {
    
    this->features = example.mutable_features();
    this->feature = features->mutable_feature();
    
  }
  
  void set_int_var (std::string var_name, Rcpp::IntegerVector values) {
    
    tensorflow::Feature feat;
    auto feat_values = feat.mutable_int64_list();
    
    for (int i=0; i<values.length(); i++) {
      feat_values->add_value(values[i]);
    }
    
    this->feature->insert(pair_type(var_name, feat));
    
  }
  
  std::string serialize_to_string () {
    std::string out;
    this->example.SerializeToString(&out);
    
    return out;
  }
  
  void clear () {
    this->features->clear_feature();
  }
  
};