#pragma once
#include "../../common/include/cipher.hpp"
#include "../../common/include/echo.hpp" // EchoRequest, EchoResponse
#include "../../common/include/header.hpp"
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class EchoHandler {
public:
  EchoHandler(int socket, std::string username, std::string password) {
    _socket = socket;
    _username = username;
    _password = password;
    _logged_in = true;
  }

  ~EchoHandler() { close(); }

  void close() {
    ::close(_socket);
    _logged_in = false;
  }

  void handle() {
    if (_logged_in) {
      char buffer[1024];
      int bytesRead = read(_socket, buffer, 1024);

      if (bytesRead == 0) {
        // Client has closed the connection
        std::cout << "Client disconnected" << std::endl;
        _logged_in = false;
        return; // Exit the handle method
      } else if (bytesRead < 0) {
        // An error occurred
        std::cerr << "Error reading from socket" << std::endl;
        _logged_in = false;
        return; // Exit the handle method
      }
      Header header = Header(std::vector<char>(buffer, buffer + 1024));
      if (header.type == Header::HeaderType::ECHO_REQUEST) {
        EchoRequest request =
            EchoRequest(std::vector<char>(buffer, buffer + 1024));
        send(request.message, request.header.seq);
        std::cout << "Echo request accepted" << std::endl;
      }
    }
  }
  bool send(std::string message, unsigned int seq) {
    if (_logged_in) {
      message = cipher::decrypt(
          message, cipher::calculate_initial_value(_username, _password, seq));
      EchoResponse response = EchoResponse(message);
      std::vector<char> serializedData = response.serialize();
      ::send(_socket, serializedData.data(), serializedData.size(), 0);
      return true;
    }
    return false;
  }

  bool is_logged_in() { return _logged_in; }

private:
  bool _logged_in;
  int _socket;
  std::string _username;
  std::string _password;
};
