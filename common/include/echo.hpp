#pragma once

#include "header.hpp"
#include <string>
#include <cstring>
#include <vector>


struct EchoRequest{
    Header header;
    unsigned int size;
    std::string message;

    EchoRequest();

    EchoRequest(std::string message, unsigned int seq);

    EchoRequest(std::vector<char> data);

    std::vector<char> serialize();
};

struct EchoResponse{
    Header header;
    unsigned int size;
    std::string message;

    EchoResponse();

    EchoResponse(std::string message);

    EchoResponse(std::vector<char> data);

    std::vector<char> serialize();
};
