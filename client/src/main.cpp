#include "../include/echoConnection.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
void showHelp() {
  std::cout << "Usage: echoClient <mode> <options>\n";
  std::cout << "Modes:\n";
  std::cout << "  benchmark <message> - Sends a message repeatedly and "
               "measures performance.\n";
  std::cout << "  interactive - Runs in interactive mode.\n";
  std::cout << "  test <file> - Runs a test from a file.\n";
}
void runTestMode(const std::string &filePath) {
  EchoConnection connection = EchoConnection(8989);
  connection.login("user", "pass");

  std::ifstream testFile(filePath);
  if (!testFile.is_open()) {
    std::cerr << "Failed to open file: " << filePath << std::endl;
    return;
  }

  std::string line;
  while (std::getline(testFile, line)) {
    connection.send(line);
    std::string response = connection.receive();

    if (response == line) {
      std::cout << "Test passed for line: " << line << std::endl;
    } else {
      std::cerr << "Test failed for line: " << line
                << ". Received: " << response << std::endl;
    }
  }

  connection.close();
  testFile.close();
}
int main(int argc, char const *argv[]) {
  if (argc < 2) {
    showHelp();
    return 1;
  }

  std::string mode = argv[1];
  try {
    if (mode == "test" && argc > 2) {
      runTestMode(argv[2]);
    } else if (mode == "interative") {

      EchoConnection connection = EchoConnection(8989);
      connection.login("username", "password");
      while (true) {
        std::string message;
        std::cin >> message;
        if (message == "exit") {
          break;
        }
        connection.send(message);
        std::cout << connection.receive() << std::endl;
      }
      connection.close();
    } else if (mode == "benchmark" && argc > 2) {
      std::string message = argv[2];
      EchoConnection connection = EchoConnection(8989);
      connection.login("username", "password");
      int count = 100000;
      auto start = std::chrono::high_resolution_clock::now();
      for (int i = 0; i < count; ++i) {
        connection.send(message);
        std::cout << connection.receive() << std::endl;
      }
      auto end = std::chrono::high_resolution_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              .count();
      std::cout << "Messages per second: " << (count * 1000) / duration
                << std::endl;
      std::cout << "Time per message: " << duration / count << std::endl;
      std::cout << "Total time: " << duration << std::endl;
      connection.close();
    } else {
      std::cerr << "Invalid mode: " << mode << std::endl;
      showHelp();
      return 1;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
