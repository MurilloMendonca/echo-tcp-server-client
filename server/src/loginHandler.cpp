#include "../include/loginHandler.hpp"

#include <iostream>
#include <string>

#include "../../common/include/header.hpp"
#include "../../common/include/login.hpp"
#include "../include/echoHandler.hpp"
#include "../include/loginValidator.hpp"
LoginHandler::LoginHandler(int port) {
    if (open(port)) {
      _connected = true;
    } else {
      _connected = false;
    }
  }

  LoginHandler::~LoginHandler() {
      for(auto& t : _threads) {
          t.join();
      }
    if (_connected) {
      close();
    }
  }

  bool LoginHandler::open(int port) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    server_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    // Setting up the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind the socket
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    _server_fd = server_fd;
    _address = address;
    return true;
  }

  void LoginHandler::listen() {
    while (_connected) {
      std::cout << "Listening" << std::endl;
      ::listen(_server_fd, 3);
      int addrlen = sizeof(_address);
      int new_socket = accept(_server_fd, (struct sockaddr *)&_address,
                              (socklen_t *)&addrlen);
      char buffer[1024];
      ::read(new_socket, buffer, 1024);
      std::cout << "Received data" << std::endl;

      Header header = Header(std::vector<char>(buffer, buffer + 1024));
      header.print();
      if (header.type == Header::HeaderType::LOGIN_REQUEST) {
        LoginRequest request =
            LoginRequest(std::vector<char>(buffer, buffer + 1024));
        std::cout << "Received login request" << std::endl;
        std::cout << "Username: " << request.username << std::endl;
        std::cout << "Password: " << request.password << std::endl;
        ExampleLoginValidator validator;
        if (!validator.validate(request.username, request.password)) {
          std::cout << "Login failed" << std::endl;
          LoginResponse response = LoginResponse(LoginResponse::LoginStatus::FAILED);
          std::vector<char> serializedData = response.serialize();
          ::send(new_socket, serializedData.data(), serializedData.size(), 0);
          continue;
        }
        LoginResponse response = LoginResponse(LoginResponse::LoginStatus::OK);
        std::vector<char> serializedData = response.serialize();
        ::send(new_socket, serializedData.data(), serializedData.size(), 0);
        std::thread t([request, new_socket]() {
          std::cout << "Thread started for socket: " << new_socket << std::endl;
          EchoHandler echoHandler(new_socket, request.username,
                                  request.password);
          while (echoHandler.is_logged_in()) {
            echoHandler.handle();
          }
        });
        t.detach();
        this->_threads.push_back(std::move(t));
      }
    }
  }

  void LoginHandler::close() {
    if (_connected) {
      ::close(_server_fd);
      _connected = false;
    }
  }
