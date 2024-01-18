#include "../include/echoConnection.hpp"

EchoConnection::EchoConnection(int socket) {
  _logged_in = false;
  if (open(socket)) {
    _connected = true;
  } else {
    _connected = false;
  }
}

EchoConnection::~EchoConnection() {
  if (_connected) {
    close();
  }
}

bool EchoConnection::send(std::string message) {
  if (_connected) {
    message = cipher::encrypt(
        message, cipher::calculate_initial_value(_username, _password, _seq));
    EchoRequest request = EchoRequest(message, _seq);
    std::vector<char> serializedData = request.serialize();
    ::send(_socket, serializedData.data(), serializedData.size(), 0);
    _seq++;
    return true;
  }
  return false;
}

bool EchoConnection::login(std::string username, std::string password) {
  if (!_connected) {
    std::cout << "Not connected" << std::endl;
    return false;
  }
  if (_logged_in) {
    std::cout << "Already connected" << std::endl;
    return true;
  }

  LoginRequest request = LoginRequest(username, password);
  std::cout << "Sending login request" << std::endl;
  request.header.print();
  std::cout<<request.username<<std::endl;
  std::cout<<request.password<<std::endl;
  std::vector<char> serializedData = request.serialize();
  std::cout << "Serialized data size: " << serializedData.size() << std::endl;
  ::send(_socket, serializedData.data(), serializedData.size(), 0);
  std::cout << "Sent login request" << std::endl;

  char buffer[1024];
  ::read(_socket, buffer, 1024);
  LoginResponse response =
      LoginResponse(std::vector<char>(buffer, buffer + 1024));
  std::cout << "Received login response" << std::endl;
  std::cout << "Status: " << (unsigned int)response.status << std::endl;

  if (response.status == LoginResponse::LoginStatus::OK) {
    _logged_in = true;
    std::cout << "Login successful" << std::endl;
    _username = username;
    _password = password;
    _seq = 0;
    return true;
  }
  std::cout << "Login failed" << std::endl;
  return false;
}
std::string EchoConnection::receive() {
  if (_connected) {
    char buffer[1024] = {0};
    read(_socket, buffer, 1024);
    EchoResponse response =
        EchoResponse(std::vector<char>(buffer, buffer + 1024));
    return response.message;
  }
  return "";
}
void EchoConnection::close() { ::close(_socket); }
bool EchoConnection::open(int port) {
  int sock = 0;
  struct sockaddr_in serv_addr;

  // Create socket
  if ((sock = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "Socket creation error" << std::endl;
    return false;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cout << "Invalid address" << std::endl;
    return false;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "Connection Failed" << std::endl;
    return false;
  }
  _socket = sock;
  return true;
}
bool EchoConnection::isOpen() { return _connected; }
