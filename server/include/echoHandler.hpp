#pragma once
#include <string>

class EchoHandler {
public:
  EchoHandler(int socket, std::string username, std::string password);

  ~EchoHandler();

  void close();

  void handle();
  bool send(std::string message, unsigned int seq);

  bool is_logged_in();

private:
  bool _logged_in;
  int _socket;
  std::string _username;
  std::string _password;
};
