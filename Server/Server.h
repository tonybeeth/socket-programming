
#ifndef SOCKET_PROGRAMMING_SERVER_H
#define SOCKET_PROGRAMMING_SERVER_H

#include <Host.h>

class Server : public virtual Host {
public:
    Server();

    Server(const std::string &ip, const std::string &port);

    virtual void connectClient() = 0;

    virtual void setup() = 0;

    virtual void disconnectClient() = 0;
};


#endif //SOCKET_PROGRAMMING_SERVER_H
