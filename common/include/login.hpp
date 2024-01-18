#pragma once

#include "header.hpp"
#include <stdexcept>
#include <string>
#include <cstring>
#include <vector>

struct LoginRequest{
    Header header;
    std::string username;
    std::string password;

    LoginRequest();
    LoginRequest(std::string username, std::string password);

    LoginRequest(std::vector<char> data);
    
    std::vector<char> serialize();    
    
};


struct LoginResponse{
    enum class LoginStatus: unsigned int{
        FAILED = 0,
        OK = 1,
    };
    Header header;
    LoginStatus status;
    
    LoginResponse();
    LoginResponse(LoginStatus status);

    LoginResponse(std::vector<char> data);

    std::vector<char> serialize();
};
