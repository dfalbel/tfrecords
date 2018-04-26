// [[Rcpp::depends(BH)]]
// [[Rcpp::depends(RcppEigen)]]
// [[Rcpp::depends(progress)]]

#include "example.pb.h" // needs to be included first because of the Free macro
#include "RcppEigen.h"
#include "RecordWriter.h"
#include "Example.h"
#include <RProgress.h>
#include <vector>

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

struct DataTypes {
  Rcpp::NumericVector num_vec;
  Rcpp::NumericMatrix num_mat;
  Rcpp::IntegerVector int_vec;
  Rcpp::IntegerMatrix int_mat;
  Eigen::SparseMatrix<double, Eigen::RowMajor> sp_mat;
};

struct Desc {
  std::string clss;
  std::string type;
  Rcpp::IntegerVector dim;
};

// [[Rcpp::export]]
bool write_tfrecords_ (const Rcpp::List &data, 
                       const Rcpp::List &description, 
                       const int n, 
                       const std::string path,
                       bool interactive) {
  
  int len = data.length();
  std::vector<std::string> var_names = data.names();
  
  Example example;
  RecordWriter writer(path);
  
  // casting list elements
  
  std::vector<Desc> d(len);
  std::vector<DataTypes> data_(len);
  
  for (int j=0; j<len; j++) {
    Rcpp::List desc = description[j];
    d[j].clss = Rcpp::as<std::string>(desc["class"]);
    d[j].type = Rcpp::as<std::string>(desc["type"]);
    
    std::string clss = d[j].clss;
    std::string type = d[j].type;
    
    if (clss == "matrix") {
      if (type == "integer") {
        data_[j].int_mat = Rcpp::as<Rcpp::IntegerMatrix>(data[j]);
      } else if (type == "double") {
        data_[j].num_mat = Rcpp::as<Rcpp::NumericMatrix>(data[j]);
      }
    } else if (clss == "dgCMatrix") {
      data_[j].sp_mat = Rcpp::as<Eigen::MappedSparseMatrix<double>>(data[j]);
    } else if (clss == "array") {
      d[j].dim = desc["dimension"];
      if (type == "integer") {
        data_[j].int_vec = Rcpp::as<Rcpp::IntegerVector>(data[j]);
      } else if (type == "double") {
        data_[j].num_vec = Rcpp::as<Rcpp::NumericVector>(data[j]);
      }
    }
  }
  
  // Iteration over all elements
  
  RProgress::RProgress pb(" :current / :total [:bar] ");
  if (interactive) {
    pb.set_total(n);
    pb.tick(0);  
  }
  
  
  for (int i=0; i<n; i++) {
    
    Rcpp::checkUserInterrupt();
    
    for (int j=0; j<len; j++) {
      
      std::string clss = d[j].clss;
      std::string type = d[j].type;
      
      if (clss == "matrix") {
        if (type == "integer") {
          example.set_int_var(var_names[j], data_[j].int_mat(i, _));
        } else if (type == "double") {
          example.set_float_var(var_names[j], data_[j].num_mat(i, _));
        } else {
          Rcpp::stop("Invalid matrix type: ", type);
        }
  
      } else if (clss == "dgCMatrix") {
      
        Eigen::SparseVector<double> row = data_[j].sp_mat.row(i);
        
        std::vector<int> index;
        std::vector<double> value;
        
        for (Eigen::SparseVector<double>::InnerIterator i_(row); i_; ++i_){
          index.push_back(i_.index());
          value.push_back(i_.value());
        }
        
        example.set_int_var("index_" + var_names[j], Rcpp::wrap(index));
        example.set_float_var("value_" + var_names[j], Rcpp::wrap(value));
        
      } else if (clss == "array") {
        
        Rcpp::IntegerVector dim = d[j].dim;
        
        int dim_size = 1;
        for (int p=1; p<dim.length(); p++) { // starts at 1 to avoid the first dim
          dim_size = dim_size*dim[p]; 
        }
        
        if (type == "integer") {
          
          Rcpp::IntegerVector x = data_[j].int_vec;
          Rcpp::IntegerVector res(dim_size);
          
          for (int l = 0; l<dim_size; l++) {
            res[l] = x[i*dim_size + l];
          }
          
          example.set_int_var(var_names[j], res);
          
        } else if (type == "double") {
          
          Rcpp::NumericVector x = data_[j].num_vec;
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
    
    if (interactive) {
      pb.tick(); 
    }
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
