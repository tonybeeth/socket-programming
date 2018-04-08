#include "WClient.h"

addrinfo* WClient::getServerInfo(const std::string& ip, const std::string& port) {
    addrinfo *result = nullptr, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
    if(iResult != 0) {
        fprintf(stderr, "getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    return result;
}

void WClient::connectServer(const std::string& ip, const std::string& port) {
    /*if (!server) {
        fprintf(stderr, "Cannot connect to NULL server\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }*/

    auto serverInfo = getServerInfo(ip, port);
    SOCKET connSocket = createSocket(serverInfo);
    connectSocket(connSocket, serverInfo);

    freeaddrinfo(serverInfo);

    //Update current connected socket
    this->connectedSocket = connSocket;
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

WClient::~WClient() = default;

void WClient::disconnectServer() {
    closeSocket(connectedSocket);
}
