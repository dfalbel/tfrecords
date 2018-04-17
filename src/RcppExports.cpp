// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// write_tfrecord
bool write_tfrecord(Rcpp::IntegerMatrix x, std::string path);
RcppExport SEXP _tfrecords_write_tfrecord(SEXP xSEXP, SEXP pathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::IntegerMatrix >::type x(xSEXP);
    Rcpp::traits::input_parameter< std::string >::type path(pathSEXP);
    rcpp_result_gen = Rcpp::wrap(write_tfrecord(x, path));
    return rcpp_result_gen;
END_RCPP
}
// write_tfrecords_
bool write_tfrecords_(Rcpp::List data, Rcpp::List desc, int n, std::string path);
RcppExport SEXP _tfrecords_write_tfrecords_(SEXP dataSEXP, SEXP descSEXP, SEXP nSEXP, SEXP pathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::List >::type data(dataSEXP);
    Rcpp::traits::input_parameter< Rcpp::List >::type desc(descSEXP);
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< std::string >::type path(pathSEXP);
    rcpp_result_gen = Rcpp::wrap(write_tfrecords_(data, desc, n, path));
    return rcpp_result_gen;
END_RCPP
}
// tfrecord_shutdown
bool tfrecord_shutdown();
RcppExport SEXP _tfrecords_tfrecord_shutdown() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(tfrecord_shutdown());
    return rcpp_result_gen;
END_RCPP
}
// write_test_example
std::string write_test_example(std::string path);
RcppExport SEXP _tfrecords_write_test_example(SEXP pathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type path(pathSEXP);
    rcpp_result_gen = Rcpp::wrap(write_test_example(path));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_tfrecords_write_tfrecord", (DL_FUNC) &_tfrecords_write_tfrecord, 2},
    {"_tfrecords_write_tfrecords_", (DL_FUNC) &_tfrecords_write_tfrecords_, 4},
    {"_tfrecords_tfrecord_shutdown", (DL_FUNC) &_tfrecords_tfrecord_shutdown, 0},
    {"_tfrecords_write_test_example", (DL_FUNC) &_tfrecords_write_test_example, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_tfrecords(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
