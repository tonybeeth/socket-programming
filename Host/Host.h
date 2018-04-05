
#ifndef SOCKET_PROGRAMMING_HOST_H
#define SOCKET_PROGRAMMING_HOST_H

#include <string>

#define BUFFER_SIZE_TYPE unsigned int
#define BUFFER_MAX_SIZE sizeof(BUFFER_SIZE_TYPE)

class Host {

private:
    std::string ip;
    std::string port;

public:

    Host() {}

    Host(const std::string &ip, const std::string &port) : ip(ip), port(port) {}

    const std::string &getIp() const {
        return ip;
    }

    void setIp(const std::string &ip) {
        Host::ip = ip;
    }

    const std::string &getPort() const {
        return port;
    }

    void setPort(const std::string &port) {
        Host::port = port;
    }

    std::string toString() {
        return "IP: " + ip + ", PORT: " + port;
    }

};

#endif //SOCKET_PROGRAMMING_HOST_H
