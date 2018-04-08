
#ifndef SOCKET_PROGRAMMING_PSERVER_H
#define SOCKET_PROGRAMMING_PSERVER_H

#include <Client.h>
#include <Server.h>
#include <PHost.h>

#define MAX_CLIENT_QUEUE_SIZE 5

class PServer : public PHost, public Server{
public:
    PServer();

    PServer(const std::string &ip, const std::string &port);

    void connectClient() override;

    void setup() override;

    void disconnectClient() override;

private:

    int listeningSocket;
};


#endif //SOCKET_PROGRAMMING_PSERVER_H
