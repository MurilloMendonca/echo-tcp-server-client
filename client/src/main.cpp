#include <iostream>
#include <string>
#include <chrono>
#include "../include/echoConnection.hpp"
int main(int argc, char const *argv[]) {
    
    //benchmark mode: send the same message N times, mesure the time, and print the messages per second
    if(argc > 1) {
        std::string message = argv[1];
        EchoConnection connection = EchoConnection(8989);
        connection.login("username", "password");
        int count = 100;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < count; ++i) {
            connection.send(message);
            std::cout<<connection.receive()<<std::endl;
            
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout<<"Messages per second: "<<(count * 1000) / duration<<std::endl;
        std::cout<<"Time per message: "<<duration / count<<std::endl;
        std::cout<<"Total time: "<<duration<<std::endl;
        connection.close();
    }
    else{ //interactive mode: send a message, receive the response, repeat until the user types "exit"
    EchoConnection connection = EchoConnection(8989);
    connection.login("username", "password");
    while(true){
        std::string message;
        std::cin>>message;
        if(message == "exit") {
            break;
        }
        connection.send(message);
        std::cout<<connection.receive()<<std::endl;
    }
    connection.close();
    }
    return 0;
}

