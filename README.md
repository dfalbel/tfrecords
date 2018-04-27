# tfrecords

Write tfrecords from R

[![Travis-CI Build Status](https://travis-ci.org/dfalbel/tfrecords.svg?branch=master)](https://travis-ci.org/dfalbel/tfrecords)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/dfalbel/tfrecords?branch=master&svg=true)](https://ci.appveyor.com/project/dfalbel/tfrecords)

## Example

`tfrecords` writes *.tfrecords* efficiently directly from R. It currently supports writing
lists of matrices (`matrix` class in R), arrays (`array` class in R) and sparse matrices (`dgCMatrix` in R).

``` r
data <- list(
  x = matrix(1:1000, nrow = 100),
  y = Matrix::rsparsematrix(nrow = 100, ncol = 10, density = 0.3),
  z = array(1:10000, dim = c(100, 10, 10)),
)
  
write_tfrecords(data, "example.tfrecords")
```

All elements of the `data` list must have the same number of *observations* - number of rows for matrices and sparse matrices and size of first dimension for arrays. This a requeriment of the .tfrecords format since we need to write each observation at a time.

## Installation

Before installing the R package you need to install Protobuf compiler and runtime libraries (version > 3) - See installation instructions below.

You can then install `tfrecords` from Github with:

``` r
# install.packages("devtools")
devtools::install_github("dfalbel/tfrecords")
```

### Ubuntu

There's not an `apt` package to install protobuf > 3.0 directly on Ubuntu so you will
need to compile it from source.

The following should just work:

```bash
curl -OL https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-cpp-3.5.1.zip
unzip protobuf-cpp-3.5.1.zip -d proto
cd proto/protobuf-3.5.1/
./configure
make
sudo make install
sudo ldconfig
```

It downloads the protobuf source code from the releases page and compile both the
runtime library and compiler from source. **Note** that the compilation takes ~15 minutes.

### MacOS

MacOS installation can be done via `brew`. Make sure to run:

```bash
brew install protobuf
```

before installing the `tfrecords` package.

### Windows

Pre built binaries are downloaded via [rwinlib](https://github.com/rwinlib/protobuf) 
in the configurations steps, so you shouldn't need to install protobuf before installing
`tfrecords` package.


