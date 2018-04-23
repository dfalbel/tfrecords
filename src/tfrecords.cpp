// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(RcppArmadillo)]]
// [[Rcpp::depends(progress)]]
#include "example.pb.h" // needs to be included first because of the Free macro
#include "armadillo.h"
#include <fstream>
#include <stddef.h>
#include <typeinfo>
#include "RecordWriter.h"
#include "Example.h"
#include <RProgress.h>
#include <vector>
#include <boost/any.hpp>

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
bool write_tfrecords_ (const Rcpp::List &data, const Rcpp::List &desc, const int n, const std::string path) {
  
  int l = data.length();
  std::vector<std::string> var_names = data.names();
  
  Example example;
  RecordWriter writer(path);
  
  RProgress::RProgress pb(" :current / :total [:bar] ");
  pb.set_total(n);
  
  std::vector<boost::any> datap(l);
  for (int j=0; j<l; j++) {
    
    Rcpp::List d = desc[j];
    std::string klass = d["class"];
    std::string type = d["type"];
    
    if ( klass == "matrix" ) {
      if ( type == "integer" ) {
        datap[j] = as<Rcpp::IntegerMatrix>(data[j]);
      } else if ( type == "double" ) {
        datap[j] = as<Rcpp::NumericMatrix>(data[j]);
      } else {
        Rcpp::stop("Invalid matrix type: ", type);
      }
    } else if (klass == "dgCMatrix") {
      datap[j] = as<arma::sp_mat>(data[j]);
    } else if (klass == "array") {
      if (type == "integer") {
        datap[j] = as<Rcpp::IntegerVector>(data[j]);
      } else if (type == "double") {
        datap[j] = as<Rcpp::NumericVector>(data[j]);
      } else {
        Rcpp::stop("Invalid array type: ", type);
      }
    } else {
      Rcpp::stop("Invalid class.");
    }
  }
  
  pb.tick(0);
  for (int i=0; i<n; i++) {
    
    Rcpp::checkUserInterrupt();
    
    for (int j=0; j<l; j++) {
      
      Rcpp::List d = desc[j];
      std::string klass = d["class"];
      std::string type = d["type"];
      
      if ( klass == "matrix" ) {
        
        
        if ( type == "integer" ) {
          
          Rcpp::IntegerMatrix x = boost::any_cast<Rcpp::IntegerMatrix>(datap[j]);
          example.set_int_var(var_names[j], x(i,_));
          
        } else if ( type == "double" ) {
          
          Rcpp::NumericMatrix x = boost::any_cast<Rcpp::NumericMatrix>(datap[j]);
          example.set_float_var(var_names[j], x(i,_));
          
        } else {
          
          Rcpp::stop("Invalid matrix type: ", type);
          
        }
        
      } else if (klass == "dgCMatrix") {
        
        arma::sp_mat x = boost::any_cast<arma::sp_mat>(datap[j]);
        arma::sp_colvec row = x.col(i);
        
        arma::sp_colvec::const_iterator start = row.begin();
        arma::sp_colvec::const_iterator end   = row.end();
      
        std::vector<int> index;
        std::vector<double> value;
        
        for (arma::sp_colvec::const_iterator it = start; it != end; ++it) {
          index.push_back(it.row());
          value.push_back(*it);
        }
        
        example.set_int_var("index_" + var_names[j], Rcpp::wrap(index));
        example.set_float_var("value_" + var_names[j], Rcpp::wrap(value));
        
        
      } else if (klass == "array") {
        
        Rcpp::IntegerVector dim = d["dimension"];
        int dim_size = 1;
        for (int p=1; p<dim.length(); p++) { // starts at 1 to avoid the first dim
          dim_size = dim_size*dim[p]; 
        }
        
        if (type == "integer") {
          
          Rcpp::IntegerVector x = boost::any_cast<Rcpp::IntegerVector>(datap[j]);
          Rcpp::IntegerVector res(dim_size);
          
          for (int l = 0; l<dim_size; l++) {
            res[l] = x[i*dim_size + l];
          }
          
          example.set_int_var(var_names[j], res);
          
        } else if (type == "double") {
          
          Rcpp::NumericVector x = boost::any_cast<Rcpp::NumericVector>(datap[j]);
          Rcpp::NumericVector res(dim_size);
          
          for (int l = 0; l<dim_size; l++) {
            res[l] = x[i*dim_size + l];
          }
          
          example.set_float_var(var_names[j], res);
          
        } else {
          
          Rcpp::stop("Invalid array type: ", type);
          
        }
        
        
      } else {
        
        Rcpp::stop("Invalid class.");
        
      }
      
    }  
    
    writer.write_record(example.serialize_to_string());
    example.clear();
    pb.tick();
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
