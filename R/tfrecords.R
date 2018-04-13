#' @useDynLib tfrecords
#' @importFrom Rcpp sourceCpp
NULL

.onUnload <- function (libpath) {
  x <- tfrecord_shutdown()
  library.dynam.unload("tfrecords", libpath)
}


#' Write tfrecords from a list of data.
#'
#' @param data list of data.
#'
#' @export
#' 
#' @examples 
#' data <- list(
#'  x = matrix(1:1000, nrow = 100),
#'  y = matrix(1:1000/2.1, nrow = 100),
#'  z = Matrix::Matrix(sample(0:1, prob = c(0.9, 0.1), size = 1000, replace = TRUE), nrow = 100)
#'  )
#'
#'
write_tfrecords <- function (data, path) {
  desc <- lapply(data, get_class_and_type)
  write_tfrecords_(data, desc, path)
}

#' Get classes and value types from an object supported.
#'
#'
get_class_and_type <- function (x) {
  
  k <- as.character(class(x))
  
  if (k == "matrix") {
    type <- typeof(x)
  } else if (k == "dgCMatrix") {
    type <- typeof(x@x)
  }
  
  list(
    class = k,
    type = type
  )
}


