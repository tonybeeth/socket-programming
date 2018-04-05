
#ifndef SOCKET_PROGRAMMING_PSERVER_H
#define SOCKET_PROGRAMMING_PSERVER_H

#include <Client.h>
#include "Server.h"

#define MAX_CLIENT_QUEUE_SIZE 5

class PServer : public Server{
public:
    PServer();

    PServer(const std::string &ip, const std::string &port);

    void connectClient();

    std::string receiveMessage();

    void sendMessage(const std::string& message);

    void setup();

private:

    char* receiveBytes(unsigned int bytes, int socket);

    void writeBytes(unsigned int bytes, int socket, const char* msg);

    int listeningSocket;

    int connectedClientSocket;
};


#endif //SOCKET_PROGRAMMING_PSERVER_H
