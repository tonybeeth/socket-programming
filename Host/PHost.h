#ifndef SOCKET_PROGRAMMING_PHOST_H
#define SOCKET_PROGRAMMING_PHOST_H

#include "Host.h"

class PHost : public virtual Host {
public:
    PHost();

    PHost(const std::string &ip, const std::string &port);

    void sendMessage(const Packet* packet, const Host *host = nullptr) override;

    Packet* receiveMessage(const Host *host = nullptr) override;

protected:
    int connectedSocket;

    void* readBytes(unsigned int bytes, int socket);

    void writeBytes(unsigned int bytes, int socket, const void* msg);

    void printError(const char *msg);

    void closeSocket(int& socket);
};


#endif //SOCKET_PROGRAMMING_PHOST_H
