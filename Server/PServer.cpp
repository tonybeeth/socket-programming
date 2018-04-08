#include "PServer.h"
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <memory>
#include <unistd.h>

PServer::PServer() : Server(), PHost(), listeningSocket(-1) {
}

PServer::PServer(const std::string &ip, const std::string &port) : Host(ip, port), Server(ip, port), listeningSocket(-1) {
}

void PServer::setup() {
    sockaddr_in serverAddress;

    listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningSocket == -1) {
        printError("socket() failed");
        exit(EXIT_FAILURE);
    }

    //Set server address properties
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(std::stoul(getPort()));

    serverAddress.sin_addr.s_addr = INADDR_ANY;

    //Bind socket to address
    if(bind(listeningSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) != 0) {
        printError("ERROR on binding");
        exit(EXIT_FAILURE);
    }

    if (listen(listeningSocket, MAX_CLIENT_QUEUE_SIZE) != 0) {
        printError("Error on listening");
        exit(EXIT_FAILURE);
    }
}

void PServer::connectClient() {
    sockaddr_in clientAddress;
    unsigned int clientAddressSize = sizeof(clientAddress);
    int connectedSocket = accept(listeningSocket, (struct sockaddr*) &clientAddress, &clientAddressSize);
    if(connectedSocket < 0) {
        printError("Error on accepting");
        exit(EXIT_FAILURE);
    }

    this->connectedSocket = connectedSocket;
    //return Client(std::to_string(clientAddress.sin_addr.s_addr), std::to_string(clientAddress.sin_port));
}

void PServer::disconnectClient() {
    closeSocket(connectedSocket);
}
