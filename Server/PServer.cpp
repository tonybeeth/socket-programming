
#include "PServer.h"
#include <PCommon.h>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <memory>
#include <unistd.h>

PServer::PServer() : Server(), listeningSocket(-1) {
}

PServer::PServer(const std::string &ip, const std::string &port) : Server(ip, port), listeningSocket(-1) {
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
    connectedClientSocket = accept(listeningSocket, (struct sockaddr*) &clientAddress, &clientAddressSize);
    if(connectedClientSocket < 0) {
        printError("Error on accepting");
        exit(EXIT_FAILURE);
    }
    //return Client(std::to_string(clientAddress.sin_addr.s_addr), std::to_string(clientAddress.sin_port));
}

std::string PServer::receiveMessage() {
    auto bufferSize = std::shared_ptr<BUFFER_SIZE_TYPE>((BUFFER_SIZE_TYPE*)receiveBytes(BUFFER_MAX_SIZE, connectedClientSocket));
    auto buffer = std::shared_ptr<char>(receiveBytes(*bufferSize, connectedClientSocket));
    return buffer.get();
}

char* PServer::receiveBytes(BUFFER_SIZE_TYPE bytes, int socket) {
    auto buffer = new char[bytes + 1];
    buffer[bytes] = '\0';

    int totalBytesReceived = 0, currentBytesReceived = 0;

    while (totalBytesReceived < bytes) {
        currentBytesReceived = read(socket, &buffer[totalBytesReceived], bytes - totalBytesReceived);
        if (currentBytesReceived == -1) {
            printError("Error receiving message");
            break;
        } else {
            totalBytesReceived += currentBytesReceived;
        }
    }
    return buffer;
}

void PServer::writeBytes(unsigned int bytes, int socket, const char* msg) {
    int totalBytesWritten = 0, currentBytesWritten = 0;

    while(totalBytesWritten < bytes) {
        currentBytesWritten = write(socket, msg, bytes);
        if(currentBytesWritten == -1) {
            printError("Error sending data");
            break;
        } else {
            totalBytesWritten += currentBytesWritten;
        }
    }
}

void PServer::sendMessage(const std::string &message) {
    auto dataSize = std::shared_ptr<int>(new int(message.size()));
    //Write size
    writeBytes(BUFFER_MAX_SIZE, connectedClientSocket, (char*)dataSize.get());
    writeBytes(*dataSize, connectedClientSocket, message.c_str());
}
