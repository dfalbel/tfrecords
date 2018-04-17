#include <Rcpp.h>
using namespace Rcpp;

class IntegerArray {
  
public:  
  
  Rcpp::IntegerVector x;
  Rcpp::IntegerVector dim;
  int dim_size = 1;
  int n_obs;
  
  IntegerArray (Rcpp::IntegerVector x, Rcpp::IntegerVector dim) {
    this->x = x;
    this->dim = dim;
    
    for (int i=1; i<dim.length(); i++) { // starts at 1 to avoid the first dim
      this->dim_size = this->dim_size*dim[i]; 
    }
    
    this->n_obs = dim[0];
    
  }
  
  IntegerVector get_obs (int i) {
    
    Rcpp::IntegerVector res(this->dim_size);
    
    for (int j=0; i<this->dim_size; j++) {
      res[i] = this->x[i + j*this->n_obs];
    }
    
    return res;
  };

};