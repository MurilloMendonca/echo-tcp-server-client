#pragma once
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

class LoginHandler {
public:
  LoginHandler(int port);

  ~LoginHandler();

  bool open(int port);

  void listen();

  void close();

private:
  bool _connected;
  int _server_fd;
  struct sockaddr_in _address;
  std::vector<std::thread> _threads;
};
