#pragma once

#include <cstdint>
#include <string>

namespace cipher {
uint32_t next_key(uint32_t key);

uint8_t checksum(std::string s);
uint32_t calculate_initial_value(std::string name, std::string password,
                                 unsigned int seq);
std::string encrypt(std::string s, uint32_t initial_value);

std::string decrypt(std::string s, uint32_t initial_value);
} // namespace cipher
