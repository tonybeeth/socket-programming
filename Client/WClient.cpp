#include "WClient.h"
#include <WCommon.h>
#include <memory>

WClient::WClient(const std::string &ip, const std::string &port) : Client(ip, port) {
    connSocket = INVALID_SOCKET;
}

WClient::WClient(): Client() {
    connSocket = INVALID_SOCKET;
}

addrinfo* WClient::getServerInfo(Server* server) {
    addrinfo *result = nullptr, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(server->getIp().c_str(), server->getPort().c_str(), &hints, &result);
    if(iResult != 0) {
        fprintf(stderr, "Server (%s). getaddrinfo failed: %d\n", server->toString(), iResult);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    return result;
}

void WClient::connectServer(Server *server) {
    if (!server) {
        fprintf(stderr, "Cannot connect to NULL server\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    auto serverInfo = getServerInfo(server);
    SOCKET connSocket = createSocket(serverInfo);
    connectSocket(connSocket, serverInfo);

    freeaddrinfo(serverInfo);

    //Update current connected socket
    WClient::connSocket = connSocket;
}

SOCKET WClient::createSocket(addrinfo* serverInfo) {
    auto connSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

    if(connSocket == INVALID_SOCKET) {
        printError("Error at socket()");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return connSocket;
}

void WClient::connectSocket(SOCKET &connSocket, addrinfo* serverInfo) {
    int iResult = connect(connSocket, serverInfo->ai_addr, serverInfo->ai_addrlen);
    if(iResult == SOCKET_ERROR) {
        printError("Error connecting socket");
        closeSocket(connSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void WClient::closeSocket(SOCKET &connSocket) {
    int result = closesocket(connSocket);
    if(result == SOCKET_ERROR) {
        printError("Error closing socket");
    }
    connSocket = INVALID_SOCKET;
}

void WClient::sendMessage(const std::string &message, Server *server) {
    if(server)
        connectServer(server);

    auto dataSize = std::make_shared<int>(message.size());
    writeBytes(BUFFER_MAX_SIZE, connSocket, (char*)dataSize.get());
    writeBytes(*dataSize, connSocket, message.c_str());
}

void WClient::writeBytes(int bytes, SOCKET socket, const char* msg) {
    int totalBytesWritten = 0, currentBytesWritten = 0;

    while(totalBytesWritten < bytes) {
        currentBytesWritten = send(socket, msg, bytes, 0);
        if(currentBytesWritten == SOCKET_ERROR) {
            printError("Error sending data");
            closeSocket(socket);
            WSACleanup();
            exit(EXIT_FAILURE);
        } else {
            totalBytesWritten += currentBytesWritten;
        }
    }
}

std::string WClient::receiveMessage() {
    auto bufferSize = std::shared_ptr<int>((int*)receiveBytes(BUFFER_MAX_SIZE, connSocket));
    auto buffer = std::shared_ptr<char>(receiveBytes(*bufferSize, connSocket));
    return buffer.get();
}

char* WClient::receiveBytes(int bytes, SOCKET socket) {
    auto buffer = new char[bytes + 1];
    buffer[bytes] = '\0';

    int totalBytesReceived = 0, currentBytesReceived = 0;

    while (totalBytesReceived < bytes) {
        currentBytesReceived = recv(socket, &buffer[totalBytesReceived], bytes - totalBytesReceived, 0);
        if (currentBytesReceived == 0) {
            fprintf(stderr, "Connection Closed\n");
            break;
        } else if (currentBytesReceived == SOCKET_ERROR) {
            printError("Error receiving message");
            break;
        } else {
            totalBytesReceived += currentBytesReceived;
        }
    }
    return buffer;
}

WClient::~WClient() = default;

void WClient::disconnectServer() {
    closeSocket(connSocket);
}

void WClient::fatalError(const std::string &) {

}
