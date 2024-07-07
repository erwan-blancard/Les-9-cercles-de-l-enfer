#include <iostream>
#include <stdexcept>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../include/LPTF_Net/LPTF_Socket.hpp"
#include "../include/LPTF_Net/LPTF_Utils.hpp"

constexpr int MAX_BUFFER_SIZE = 1024;

class Server {
private:
    std::unique_ptr<LPTF_Socket> serverSocket;

public:
    Server(int port) {
        serverSocket = std::make_unique<LPTF_Socket>(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in serverAddr;
        std::memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        serverSocket->bind(reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr));
        serverSocket->listen(5); // Backlog set to 5
    }

    void start() {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        while (true) {
            std::cout << "Waiting for new client..." << std::endl;

            int clientSockfd = serverSocket->accept(reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrLen);

            std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << ". Waiting for his response..." << std::endl;
            
            try {
                LPTF_Packet client_packet = serverSocket->recv(clientSockfd, 0);
                client_packet.print_specs();
                std::cout << "Received from client: \"" << std::string((const char *)client_packet.get_content(), client_packet.get_header().length) << '\"' << std::endl;
            } catch (std::runtime_error &ex) {
                std::cout << "Error when receiving client response: " << ex.what() << std::endl;
                std::cout << "Closing connection..." << std::endl;

                close(clientSockfd);
                continue;
            }

            std::cout << "Sending message to client... ";

            LPTF_Packet message_packet = build_message_packet("Hello from server!");
            message_packet.print_specs();

            int ret = serverSocket->send(clientSockfd, message_packet, 0);
            if (ret == -1)
                std::cout << "Failed!" << std::endl;
            else
                std::cout << "Success!" << std::endl;

            std::cout << "Closing connection..." << std::endl;

            close(clientSockfd);

        }

    }
};

int main() {
    try {
        Server server(12345);
        server.start();
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
