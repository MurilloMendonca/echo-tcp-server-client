#include "../include/cipher.hpp"
namespace cipher {
uint32_t next_key(uint32_t key) {
  return (key * 1103515245 + 12345) % 0x7FFFFFFF;
}

uint8_t checksum(std::string s) {
  uint8_t sum = 0;
  for (char c : s) {
    sum += c;
  }
  return sum;
}
uint32_t calculate_initial_value(std::string name, std::string password, unsigned int seq) {
    uint32_t user_sum = checksum(name);
    uint32_t password_sum = checksum(password);
    return (seq << 16) | (user_sum << 8) | password_sum;
}
 std::string encrypt(std::string s, uint32_t initial_value) {
  std::string result;
  uint32_t key = next_key(initial_value);
  for (char c : s) {
    result += c ^ (key%256);
    key = next_key(key);
  }
  return result;
 }

std::string decrypt(std::string s, uint32_t initial_value) {
  std::string result;
  uint32_t key = next_key(initial_value);
  for (char c : s) {
    result += c ^ (key%256);
    key = next_key(key);
  }
  return result;
}
} // namespace Cipher
