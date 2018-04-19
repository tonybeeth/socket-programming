#ifndef SOCKET_PROGRAMMING_HOST_H
#define SOCKET_PROGRAMMING_HOST_H

#include <string>
#include <Packet.h>

#define BUFFER_SIZE_TYPE unsigned int
#define BUFFER_MAX_SIZE sizeof(BUFFER_SIZE_TYPE)

class Host {

private:
    std::string ip;
    std::string port;

public:
    Host();

    Host(const std::string &ip, const std::string &port);

    const std::string &getIp() const;

    void setIp(const std::string &ip);

    const std::string &getPort() const;

    void setPort(const std::string &port);

    std::string toString();

    virtual void sendMessage(const Packet* packet, const Host* host = nullptr) = 0;

    virtual Packet* receiveMessage(const Host* host = nullptr) = 0;
};

#endif //SOCKET_PROGRAMMING_HOST_H
