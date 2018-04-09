#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
std::string raw_to_char_ (const RawVector &x) {
  Rcpp::Rcout << "Byte array size: " << x.length() << "\n";
  std::string s(x, strlen(x));
  return s;
}
