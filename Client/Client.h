
#ifndef SOCKET_PROGRAMMING_CLIENT_H
#define SOCKET_PROGRAMMING_CLIENT_H

#include <Host.h>
#include <Server.h>

class Client : public Host{
public:
    Client();

    Client(const std::string &ip, const std::string &port);

    virtual void connectServer(Server *server) = 0;
};


#endif //SOCKET_PROGRAMMING_CLIENT_H
