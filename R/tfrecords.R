#' @useDynLib tfrecords
#' @importFrom Rcpp sourceCpp
NULL

.onUnload <- function (libpath) {
  x <- tfrecord_shutdown()
  library.dynam.unload("tfrecords", libpath)
}



