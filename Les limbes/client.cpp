#include <iostream>
#include <stdexcept>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "LPTF_Socket.h"

constexpr int MAX_BUFFER_SIZE = 1024;

class Client {
private:
    std::unique_ptr<LPTF_Socket> clientSocket;

public:
    Client(const std::string &serverIP, int port) {
        clientSocket = std::make_unique<LPTF_Socket>(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in serverAddr;
        std::memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
        serverAddr.sin_port = htons(port);

        clientSocket->connect(reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr));
    }

    void send(const std::string &message) {
        clientSocket->send(message.c_str(), message.size(), 0);

        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytesReceived = clientSocket->recv(buffer, MAX_BUFFER_SIZE, 0);
        std::cout << "Received from server: " << std::string(buffer, bytesReceived) << std::endl;
    }
};

int main() {
    try {
        Client client("127.0.0.1", 12345);
        client.send("Hello from client!");
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
