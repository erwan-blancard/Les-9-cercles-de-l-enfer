#include <iostream>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <vector>
#include <thread>

#include "../include/LPTF_Net/LPTF_Socket.hpp"
#include "../include/LPTF_Net/LPTF_Utils.hpp"


class Server {
private:
    std::unique_ptr<LPTF_Socket> serverSocket;
    // std::vector<std::thread> handlers;

public:
    Server(int port) {
        serverSocket = std::make_unique<LPTF_Socket>();
        // handlers = std::vector<std::thread>();

        struct sockaddr_in serverAddr;
        std::memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        serverSocket->bind(reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr));
        serverSocket->listen(5); // Backlog set to 5
    }

    void start() {
        while (true) {
            // limit number of handlers (test)
            // if (handlers.size() >= 10) {
            //     std::cout << "Reached handler limit" << std::endl;
            //     continue;
            // }

            std::cout << "Waiting for new client..." << std::endl;
            struct sockaddr_in clientAddr;
            socklen_t clientAddrLen = sizeof(clientAddr);
            int clientSockfd = serverSocket->accept(reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrLen);

            if (clientSockfd == -1) throw std::runtime_error("Error on accept connection !");

            handle_client(clientSockfd, clientAddr, clientAddrLen);

            // std::thread handler(&Server::handle_client, this, clientSockfd, clientAddr, clientAddrLen);
            // handlers.push_back(handler);    // error: use of deleted function ‘std::thread::thread(const std::thread&)’
        }
    }

    void handle_client(int clientSockfd, struct sockaddr_in clientAddr, socklen_t clientAddrLen) {
        std::cout << "Handling client: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " (" << clientAddrLen << ")" << std::endl;

        try {
            LPTF_Packet packet;
            LPTF_Packet resp;
            ssize_t ret;
            // ask client to return the host's infos
            packet = build_command_packet(HOST_INFO_COMMAND, "");
            
            ret = serverSocket->send(clientSockfd, packet, 0);

            if (ret == -1) throw std::runtime_error("Error when sending host info cmd !");
            // listen for his response
            resp = serverSocket->recv(clientSockfd, 0);

            if (resp.type() == REPLY_PACKET)
                std::cout << "Response for host info cmd: " << get_reply_content_from_reply_packet(resp) << std::endl;
            else if (resp.type() == ERROR_PACKET)
                std::cout << "Client Error for host info cmd: " << get_error_content_from_error_packet(resp)
                          << " (code " << get_error_code_from_error_packet(resp) << ")" << std::endl;
            else
                std::cout << "Unexpected responce packet for host info cmd!" << std::endl;

            // ask client to list running processes
            packet = build_command_packet(LIST_PROC_COMMAND, "");
            ret = serverSocket->send(clientSockfd, packet, 0);

            if (ret == -1) throw std::runtime_error("Error when sending list proccesses cmd !");
            // listen for his response
            resp = serverSocket->recv(clientSockfd, 0);

            if (resp.type() == REPLY_PACKET)
                std::cout << "Response for list proccesses cmd: " << get_reply_content_from_reply_packet(resp) << std::endl;
            else if (resp.type() == ERROR_PACKET)
                std::cout << "Client Error for list proccesses cmd: " << get_error_content_from_error_packet(resp)
                          << " (code " << get_error_code_from_error_packet(resp) << ")" << std::endl;
            else
                std::cout << "Unexpected responce packet for list proccesses cmd!" << std::endl;

            // ask client to list running processes
            packet = build_command_packet(SHELL_COMMAND, "echox \"Hello from server !\" >> remote.txt");
            ret = serverSocket->send(clientSockfd, packet, 0);

            if (ret == -1) throw std::runtime_error("Error when sending shell cmd !");
            // listen for his response
            resp = serverSocket->recv(clientSockfd, 0);

            if (resp.type() == REPLY_PACKET)
                std::cout << "Response for shell cmd: " << get_reply_content_from_reply_packet(resp) << std::endl;
            else if (resp.type() == ERROR_PACKET)
                std::cout << "Client Error for shell cmd: " << get_error_content_from_error_packet(resp)
                          << " (code " << get_error_code_from_error_packet(resp) << ")" << std::endl;
            else
                std::cout << "Unexpected responce packet for shell cmd!" << std::endl;

        } catch (const std::exception &ex) {
            std::cout << "Error when handling client " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " : " << ex.what() << std::endl;    
        }

        std::cout << "Closing client connection" << std::endl;
        close(clientSockfd);
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