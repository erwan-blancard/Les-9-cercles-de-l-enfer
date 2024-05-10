#include <iostream>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <sstream>

constexpr int MAX_BUFFER_SIZE = 1024;

class LPTF_Socket {
private:
    int sockfd;

public:
    LPTF_Socket(int domain, int type, int protocol) {
        sockfd = socket(domain, type, protocol);
        if (sockfd == -1) {
            throw std::runtime_error("Failed to create socket");
        }
    }

    ~LPTF_Socket() {
        close(sockfd);
    }

    void bind(const struct sockaddr *addr, socklen_t addrlen) {
        if (::bind(sockfd, addr, addrlen) == -1) {
            throw std::runtime_error("Failed to bind socket");
        }
    }

    void listen(int backlog) {
        if (::listen(sockfd, backlog) == -1) {
            throw std::runtime_error("Failed to listen on socket");
        }
    }

    int accept(struct sockaddr *addr, socklen_t *addrlen) {
        int new_sockfd = ::accept(sockfd, addr, addrlen);
        if (new_sockfd == -1) {
            throw std::runtime_error("Failed to accept connection");
        }
        return new_sockfd;
    }

    ssize_t send(const void *buf, size_t len, int flags) {
        return ::send(sockfd, buf, len, flags);
    }

    ssize_t recv(void *buf, size_t len, int flags) {
        return ::recv(sockfd, buf, len, flags);
    }
};

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
        int clientSockfd = serverSocket->accept(reinterpret_cast<struct sockaddr *>(&clientAddr), &clientAddrLen);

        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytesReceived = serverSocket->recv(buffer, MAX_BUFFER_SIZE, 0);
        std::string command(buffer, bytesReceived);

        std::string response;
        if (command == "GET_HOST_INFO") {
            response = getHostInfo();
        } else if (command == "GET_RUNNING_PROCESSES") {
            response = getRunningProcesses();
        } else if (command == "EXECUTE_COMMAND") {
            response = executeCommand("ls"); 
        } else {
            response = "Invalid command";
        }

        serverSocket->send(response.c_str(), response.size(), 0);

        close(clientSockfd);
    }

private:
    std::string getHostInfo() {
        struct utsname info;
        if (uname(&info) != 0) {
            return "Failed to retrieve host information";
        }
        std::string hostInfo = "Hostname: " + std::string(info.nodename) + "\n";
        hostInfo += "Username: " + std::string(getpwuid(getuid())->pw_name) + "\n";
        hostInfo += "Operating System: " + std::string(info.sysname);
        return hostInfo;
    }

    std::string getRunningProcesses() {
        std::stringstream processList;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir("/proc")) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                const char *d_name = ent->d_name;
                if (isdigit(d_name[0])) {
                    processList << d_name << std::endl;
                }
            }
            closedir(dir);
        } else {
            return "Failed to retrieve running processes";
        }
        return processList.str();
    }

    std::string executeCommand(const std::string &command) {
        std::stringstream result;
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            return "Failed to execute command";
        }
        char buffer[MAX_BUFFER_SIZE];
        while (!feof(pipe)) {
            if (fgets(buffer, MAX_BUFFER_SIZE, pipe) != NULL) {
                result << buffer;
            }
        }
        pclose(pipe);
        return result.str();
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