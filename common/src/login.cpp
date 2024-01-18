#include "../include/login.hpp"

LoginRequest::LoginRequest() {
  header.type = Header::HeaderType::LOGIN_REQUEST;
}
LoginRequest::LoginRequest(std::string username, std::string password) {
  header.type = Header::HeaderType::LOGIN_REQUEST;
  this->username = username;
  this->password = password;
}

LoginRequest::LoginRequest(std::vector<char> data) {
  header = Header(data);
  int offset = sizeof(header.seq) + sizeof(header.size) + sizeof(header.type);
  char username[32];
  std::strncpy(username, data.data() + offset, 31);
  offset += 32;
  char password[32];
  std::strncpy(password, data.data() + offset, 31);
  this->username = std::string(username);
  this->password = std::string(password);
}

std::vector<char> LoginRequest::serialize() {
  if (this->username.size() > 31 || this->password.size() > 31) {
    throw std::runtime_error("Username or password too long");
  }
  std::vector<char> data;
  int size =
      sizeof(header.type) + sizeof(header.size) + sizeof(header.seq) + 32 + 32;
  header.size = size;
  data = header.serialize();
  char username[32];
  std::strncpy(username, this->username.c_str(), 31);
  char password[32];
  std::strncpy(password, this->password.c_str(), 31);

  data.insert(data.end(), username, username + 32);
  data.insert(data.end(), password, password + 32);

  return data;
}

LoginResponse::LoginResponse() {
  header.type = Header::HeaderType::LOGIN_RESPONSE;
}
LoginResponse::LoginResponse(LoginStatus status) {
  header.type = Header::HeaderType::LOGIN_RESPONSE;
  this->status = status;
}

LoginResponse::LoginResponse(std::vector<char> data) {
  header = Header(data);
  int offset = sizeof(header.seq) + sizeof(header.size) + sizeof(header.type);

  unsigned int status = 0;
  for (int i = 0; i < sizeof(this->status); ++i) {
    status |= (unsigned int)((unsigned char)data[i + offset]) << (i * 8);
  }
  this->status = (LoginStatus)status;
}

std::vector<char> LoginResponse::serialize() {
  std::vector<char> data;
  int size = sizeof(header.type) + sizeof(header.size) + sizeof(header.seq) +
             sizeof(this->status);
  header.size = size;
  data = header.serialize();
  unsigned int status = (unsigned int)this->status;
  for (int i = 0; i < sizeof(this->status); ++i) {
    data.push_back((status >> (i * 8)) & 0xFF);
  }
  return data;
}
