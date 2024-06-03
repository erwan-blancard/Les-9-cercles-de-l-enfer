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
    LPTF_Socket();

    LPTF_Socket(int domain, int type, int protocol);
    
    LPTF_Socket(const LPTF_Socket &src);

    ~LPTF_Socket();

    LPTF_Socket &operator=(const LPTF_Socket& src);

    void init(int domain, int type, int protocol);

    void connect(const struct sockaddr *addr, socklen_t addrlen);

    ssize_t send(int sockfdto, const void *buf, size_t len, int flags);

    ssize_t recv(int sockfdfrom, void *buf, size_t len, int flags);

    ssize_t read(void *buf, size_t len);

    ssize_t write(const void *buf, size_t len);

    int accept(sockaddr *__restrict__ addr, socklen_t *__restrict__ addr_len);

    int bind(const sockaddr *addr, socklen_t len);

    int listen(int backlog);
};

#endif