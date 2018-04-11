#include <Rcpp.h>
#include <fstream>
#include <bitset>
#include <zlib.h>
#include "crc.hpp"
using namespace Rcpp;

// helper functions

// mask delta constant
// https://github.com/tensorflow/tensorflow/blob/754048a0453a04a761e112ae5d99c149eb9910dd/tensorflow/core/lib/hash/crc32c.h#L33
const uint32_t mask_delta = 0xa282ead8ul;

// making crc
// https://github.com/tensorflow/tensorflow/blob/754048a0453a04a761e112ae5d99c149eb9910dd/tensorflow/core/lib/hash/crc32c.h#L40
uint32_t mask(uint32_t crc) {
  return ((crc >> 15) | (crc << 17)) + mask_delta;
};

// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/lib/core/coding.cc#L43
void EncodeFixed64(char* buf, std::uint64_t value) {
  memcpy(buf, &value, sizeof(value));
}

void EncodeFixed32(char* buf, std::uint32_t value) {
  memcpy(buf, &value, sizeof(value));
}

// Get CRC32 rep
// https://stackoverflow.com/a/39381287/3297472
uint32_t crc32(const std::string& my_string) {
  boost::crc_32_type result;
  result.process_bytes(my_string.data(), my_string.length());
  return result.checksum();
};

uint32_t crc32_uint(std::uint64_t x) {
  boost::crc_32_type result;
  auto x2 = static_cast<char*>(static_cast<void*>(&x));
  result.process_bytes(x2, sizeof(x2));
  return result.checksum();
};

std::uint32_t masked_crc (char * data, std::size_t n) {
  boost::crc_32_type result;
  result.process_bytes(data, n);
  return mask(result.checksum());
}

std::string uint64toBytes (std::uint64_t x) {
  std::bitset<sizeof(x)> bit(x);
  return bit.to_string();
}

class RecordWriter {
public: 
  
  std::string path;
  std::ofstream writer;
  
  RecordWriter (std::string path) {
    this->path = path;
    this->writer.open(path); 
  }
  
  ~RecordWriter () {
    this->writer.close();
  }
  
  bool write_record (std::string data) {
    
    // Format of a single record:
    //  uint64    length
    //  uint32    masked crc of length
    //  byte      data[length]
    //  uint32    masked crc of data
    
    char length[sizeof(std::uint64_t)];
    EncodeFixed64(length, data.size());
    
    char length_crc[sizeof(std::uint32_t)];
    EncodeFixed32(length_crc, masked_crc(length, sizeof(std::uint64_t)));
    
    Rcout << sizeof(std::uint32_t) << "\n";
    
    // char header[sizeof(std::uint64_t) + sizeof(std::uint32_t)];
    // EncodeFixed64(header + 0, data.size());
    // EncodeFixed32(
    //   header + sizeof(std::uint64_t), 
    //   masked_crc(header, sizeof(std::uint64_t))
    // );
    // 
    // char footer[sizeof(std::uint32_t)];
    // EncodeFixed32(footer, mask(crc32(data)));
    
    // std::uint64_t length(sizeof(data));
    // std::uint32_t lengthCRC = mask(crc32_uint(length));
    // std::uint32_t dataCRC = mask(crc32(data));
    
    // Rcout << length << "\n";
    // Rcout << uint64toBytes(length) << "\n";
    // Rcout << data << "\n";
    // Rcout << dataCRC << "\n";
    
    this->writer.write(length, sizeof(length));
    this->writer.write(length_crc, sizeof(length));
    this->writer.write(data.c_str(), data.length());
    // this->writer.write(footer, sizeof(footer));
    // 
    return true;
  }
  
  
};