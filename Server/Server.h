
#ifndef SOCKET_PROGRAMMING_SERVER_H
#define SOCKET_PROGRAMMING_SERVER_H

#include <Host.h>

class Server : public Host {
public:
    Server();

    Server(const std::string &ip, const std::string &port);
};


#endif //SOCKET_PROGRAMMING_SERVER_H
