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


LPTF_Socket::LPTF_Socket(int domain, int type, int protocol) {
    sockfd = socket(domain, type, protocol);
    if (sockfd == -1) {
        throw std::runtime_error("Failed to create socket");
    }
}
    
LPTF_Socket::~LPTF_Socket() {
    close(sockfd);
}

void LPTF_Socket::connect(const struct sockaddr *addr, socklen_t addrlen) {
    if (::connect(sockfd, addr, addrlen) == -1) {
        throw std::runtime_error("Failed to connect to server");
    }
}

ssize_t LPTF_Socket::send(const void *buf, size_t len, int flags) {
    return ::send(sockfd, buf, len, flags);
}

ssize_t LPTF_Socket::recv(void *buf, size_t len, int flags) {
    return ::recv(sockfd, buf, len, flags);
}