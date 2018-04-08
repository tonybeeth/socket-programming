
#ifndef SOCKET_PROGRAMMING_CLIENT_H
#define SOCKET_PROGRAMMING_CLIENT_H

#include <Host.h>
#include <Server.h>

class Client : public virtual Host{
public:
    Client();

    Client(const std::string &ip, const std::string &port);

    virtual void connectServer(const std::string& ip, const std::string& port) = 0;

    virtual void disconnectServer() = 0;
};


#endif //SOCKET_PROGRAMMING_CLIENT_H
