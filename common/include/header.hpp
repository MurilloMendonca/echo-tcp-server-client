#pragma once

#include <vector>
struct Header {
  enum class HeaderType : unsigned int {
        LOGIN_REQUEST = 0, 
        LOGIN_RESPONSE = 1, 
        ECHO_REQUEST = 2, 
        ECHO_RESPONSE = 3
    };
  HeaderType type;
  unsigned int size;
  unsigned int seq;


  Header();
  Header(HeaderType type, unsigned int size, unsigned int seq);
  Header(std::vector<char> data);
  Header(char *data);
  std::vector<char> serialize();
};
