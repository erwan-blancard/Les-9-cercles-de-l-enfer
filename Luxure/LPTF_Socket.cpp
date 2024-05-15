#include <iostream>
#include <stdexcept>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "LPTF_Socket.hpp"


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

ssize_t LPTF_Socket::send(int sockfdto, const void *buf, size_t len, int flags) {
    return ::send(sockfdto, buf, len, flags);
}

ssize_t LPTF_Socket::recv(int sockfdfrom, void *buf, size_t len, int flags) {
    return ::recv(sockfdfrom, buf, len, flags);
}

ssize_t LPTF_Socket::read(void *buf, size_t len) {
    return ::read(sockfd, buf, len);
}

ssize_t LPTF_Socket::write(const void *buf, size_t len) {
    return ::write(sockfd, buf, len);
}

int LPTF_Socket::accept(sockaddr *__restrict__ addr, socklen_t *__restrict__ addr_len) {
    return ::accept(sockfd, addr, addr_len);
}

int LPTF_Socket::bind(const sockaddr *addr, socklen_t len) {
    return ::bind(sockfd, addr, len);
}

int LPTF_Socket::listen(int backlog) {
    return ::listen(sockfd, backlog);
}