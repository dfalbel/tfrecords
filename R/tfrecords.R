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
#' @param path path to write the tfrecord file.
#'
#' @export
#' 
#' @examples 
#' data <- list(
#'  x = matrix(1:1000, nrow = 100),
#'  y = matrix(1:1000/2.1, nrow = 100),
#'  z = Matrix::rsparsematrix(nrow = 100, ncol = 10, density = 0.3)
#'  )
#'  
#'  write_tfrecords(data, "example.tfrecords")
#'
write_tfrecords <- function (data, path) {
  desc <- lapply(data, get_class_and_type)
  n_obs <- get_n_obs(data)
  data <- transpose_arrays(data)
  invisible(write_tfrecords_(data, desc, n_obs, path))
}

#' Transpose arrays
#' 
#' @param data list of data
#' 
transpose_arrays <- function(data) {
  lapply(data, function(x) {
    if(is.array(x) & !is.matrix(x))
      aperm(x, length(dim(x)):1)
    else
      x
  })
}

#' Get classes and value types from an object supported.
#'
#' @param x list of data structures
#'
get_class_and_type <- function (x) {

  if (is.matrix(x)) {
    type <- typeof(x)
    k    <- "matrix"
    dimension <- NULL
  } else if (inherits(x, "dgCMatrix")) {
    type <- typeof(x@x)
    k    <- "dgCMatrix"
    dimension <- NULL
  } else if (is.array(x)) {
    type <- typeof(x)
    k    <- "array" 
    dimension <- dim(x)
  } 
  
  list(
    class = k,
    type  = type,
    dimension = dimension
  )
}

#' Number of observations
#' 
#' @param x list of data 
#'
get_n_obs <- function(x) {
  
  n_obs <- sapply(x, function(i) {
    if (inherits(i, "matrix")) {
      return(nrow(i))
    } else if (inherits(i, "dgCMatrix")) {
      return(nrow(i))
    } else if (inherits(i, "array")) {
      return(dim(i)[1])
    }
  })
  
  if(length(unique(n_obs)) > 1) {
    stop(
      "Each element of the list must have the same number of observations, but had: ", 
      paste(unique(n_obs), collapse = ", ")
      )
  }
  
  unique(n_obs)
}



