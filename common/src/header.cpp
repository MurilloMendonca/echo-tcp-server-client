#include "../include/header.hpp"
#include <iostream>

Header::Header() : type(HeaderType::LOGIN_REQUEST), size(0), seq(0) {}

Header::Header(HeaderType type, unsigned int size, unsigned int seq)
    : type(type), size(size), seq(seq) {}

Header::Header( std::vector<char> data) {
    unsigned int tempType = 0;
    for (int i = 0; i < sizeof(tempType); ++i) {
        tempType |= (unsigned int)((unsigned char)data[i]) << (i * 8);
    }
    type = static_cast<HeaderType>(tempType);

    size = 0;
    for (int i = 0; i < sizeof(size); ++i) {
        size |= (unsigned int)((unsigned char)data[i + sizeof(tempType)]) << (i * 8);
    }

    seq = 0;
    for (int i = 0; i < sizeof(seq); ++i) {
        seq |= (unsigned int)((unsigned char)data[i + sizeof(tempType) + sizeof(size)]) << (i * 8);
    }
}

std::vector<char> Header::serialize() {
    std::vector<char> data;

    unsigned int tempType = static_cast<unsigned int>(type);
    for (int i = 0; i < sizeof(tempType); ++i) {
        data.push_back((tempType >> (i * 8)) & 0xFF);
    }

    for (int i = 0; i < sizeof(size); ++i) {
        data.push_back((size >> (i * 8)) & 0xFF);
    }

    for (int i = 0; i < sizeof(seq); ++i) {
        data.push_back((seq >> (i * 8)) & 0xFF);
    }

    return data;
}

void Header::print() {
    std::cout << "Header: " << std::endl;
    std::cout << "  Type: " << static_cast<unsigned int>(type) << std::endl;
    std::cout << "  Size: " << size << std::endl;
    std::cout << "  Seq: " << seq << std::endl;
}

