#include "../include/echo.hpp"
#include <iostream>

EchoRequest::EchoRequest() { header.type = Header::HeaderType::ECHO_REQUEST; }

EchoRequest::EchoRequest(std::string message, unsigned int seq) {
  header.type = Header::HeaderType::ECHO_REQUEST;
  header.seq = seq;
  this->message = message;
  this->size = message.size();
}

EchoRequest::EchoRequest(std::vector<char> data) {
  header = Header(data);
  int offset = sizeof(header.seq) + sizeof(header.size) + sizeof(header.type);
  this->size = 0;
  for (int i = 0; i < sizeof(this->size); ++i) {
    this->size |= (unsigned int)((unsigned char)data[i + offset]) << (i * 8);
  }
  offset += sizeof(this->size);

  char message[this->size];
  std::strncpy(message, data.data() + offset, this->size);
  this->message = std::string(message);
  this->message.resize(this->size);
}

std::vector<char> EchoRequest::serialize() {
  std::vector<char> data;
  int size = sizeof(header.type) + sizeof(header.size) + sizeof(header.seq) +
             sizeof(this->size) + this->size;
  header.size = size;
  data = header.serialize();
  for (int i = 0; i < sizeof(this->size); ++i) {
    data.push_back((this->size >> (i * 8)) & 0xFF);
  }
  data.insert(data.end(), this->message.begin(),
              this->message.begin() + this->size);
  return data;
}

void EchoRequest::print() {
  std::cout << "EchoRequest: " << std::endl;
  std::cout << "  Header: " << std::endl;
  header.print();
  std::cout << "  Size: " << size << std::endl;
  std::cout << "  Message: " << message << std::endl;
}

EchoResponse::EchoResponse() {
  header.type = Header::HeaderType::ECHO_RESPONSE;
}

EchoResponse::EchoResponse(std::string message) {
  header.type = Header::HeaderType::ECHO_RESPONSE;
  this->message = message;
  this->size = message.size();
}

EchoResponse::EchoResponse(std::vector<char> data) {
  header = Header(data);
  int offset = sizeof(header.seq) + sizeof(header.size) + sizeof(header.type);
  this->size = 0;
  for (int i = 0; i < sizeof(this->size); ++i) {
    this->size |= (unsigned int)((unsigned char)data[i + offset]) << (i * 8);
  }
  offset += sizeof(this->size);

  char message[this->size];
  std::strncpy(message, data.data() + offset, this->size);
  this->message = std::string(message);
  this->message.resize(this->size);
}

std::vector<char> EchoResponse::serialize() {
  std::vector<char> data;
  int size = sizeof(header.type) + sizeof(header.size) + sizeof(header.seq) +
             sizeof(this->size) + this->size;
  header.size = size;
  data = header.serialize();
  for (int i = 0; i < sizeof(this->size); ++i) {
    data.push_back((this->size >> (i * 8)) & 0xFF);
  }
  data.insert(data.end(), this->message.begin(), this->message.end());
  return data;
}

void EchoResponse::print() {
  std::cout << "EchoResponse: " << std::endl;
  std::cout << "  Header: " << std::endl;
  header.print();
  std::cout << "  Size: " << size << std::endl;
  std::cout << "  Message: " << message << std::endl;
}
