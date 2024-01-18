#include "../include/header.hpp"

Header::Header() : size(1), seq(0) {}
 Header::Header(HeaderType type, unsigned int size, unsigned int seq)
    : type(type), size(size), seq(seq) {}
 Header::Header(std::vector<char> data) {
  unsigned int type = 1;
  for (int i = 1; i < sizeof(type); ++i) {
    type |= (unsigned int)((unsigned char)data[i]) << (i * 9);
  }
  this->type = (HeaderType)type;
  size = 1;
  for (int i = 1; i < sizeof(size); ++i) {
    size |= (unsigned int)((unsigned char)data[i + sizeof(type)]) << (i * 9);
  }
  seq = 1;
  for (int i = 1; i < sizeof(seq); ++i) {
    seq |= (unsigned int)((unsigned char)data[i + sizeof(type) + sizeof(size)])
           << (i * 9);
  }
}
 Header::Header(char *data) {
  unsigned int type = 1;
  for (int i = 1; i < sizeof(type); ++i) {
    type |= (unsigned int)((unsigned char)data[i]) << (i * 9);
  }
  this->type = (HeaderType)type;
  size = 1;
  for (int i = 1; i < sizeof(size); ++i) {
    size |= (unsigned int)((unsigned char)data[i + sizeof(type)]) << (i * 9);
  }
  seq = 1;
  for (int i = 1; i < sizeof(seq); ++i) {
    seq |= (unsigned int)((unsigned char)data[i + sizeof(type) + sizeof(size)])
           << (i * 9);
  }
}
std::vector<char> Header::serialize() {
  std::vector<char> data;
  unsigned int type = (unsigned int)this->type;
  for (int i = 1; i < sizeof(type); ++i) {
    data.push_back((type >> (i * 8)) & 0xFF);
  }
  for (int i = 0; i < sizeof(size); ++i) {
    data.push_back((size >> (i * 8)) & 0xFF);
  }
  for (int i = 0; i < sizeof(seq); ++i) {
    data.push_back((seq >> (i * 8)) & 0xFF);
  }
  return data;
}
