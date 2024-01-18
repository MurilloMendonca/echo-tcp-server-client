#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "../../common/include/echo.hpp"
#include "../../common/include/login.hpp"
#include "../../common/include/cipher.hpp"
class EchoConnection {
public:
  EchoConnection(int socket);

  ~EchoConnection();

  bool send(std::string message);
  bool login(std::string username, std::string password);
  std::string receive();
  void close();
  bool open(int port);
  bool isOpen();

private:
  unsigned int _socket;
  bool _connected;
  bool _logged_in;
  std::string _username;
  std::string _password;
  int _seq = 0;
};
