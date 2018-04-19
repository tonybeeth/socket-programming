#ifndef SOCKET_PROGRAMMING_WHOST_H
#define SOCKET_PROGRAMMING_WHOST_H

#include "Host.h"
#include <winsock2.h>
#include <ws2tcpip.h>

class WHost : public virtual Host {
public:

    WHost();

    WHost(const std::string &ip, const std::string &port);

    void sendMessage(const Packet* packet, const Host* host = nullptr) override;

    Packet* receiveMessage(const Host* host = nullptr) override;

protected:
    SOCKET connectedSocket;

    //Will keep reading from socket until all bytes are read
    void* readBytes(unsigned int bytes, SOCKET socket);

    //Will keep writing to socket until all bytes are written
    void writeBytes(unsigned int bytes, SOCKET socket, const void* msg);

    void closeSocket(SOCKET &connSocket);

    wchar_t* WSAGetLastErrorFormatted();

    void printError(const char* msg);
};


#endif //SOCKET_PROGRAMMING_WHOST_H
