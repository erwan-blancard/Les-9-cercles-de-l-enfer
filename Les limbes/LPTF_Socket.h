#ifndef LPTF_SOCKET_H
#define LPTF_SOCKET_H

#include <iostream>
#include <stdexcept>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class LPTF_Socket {
private:
    int sockfd;

public:
    LPTF_Socket(int domain, int type, int protocol);

    ~LPTF_Socket();

    void connect(const struct sockaddr *addr, socklen_t addrlen);

    ssize_t send(const void *buf, size_t len, int flags);

    ssize_t recv(void *buf, size_t len, int flags);
};

#endif