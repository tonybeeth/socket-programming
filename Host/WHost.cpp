
#include "WHost.h"
#include <memory>

WHost::WHost() : Host(), connectedSocket(INVALID_SOCKET) {}

WHost::WHost(const std::string &ip, const std::string &port) : Host(ip, port), connectedSocket(INVALID_SOCKET) {}

void WHost::sendMessage(const std::string &message, const Host* host) {
    //Currently, only one host can be connected at a time
    //Not working for multiple hosts

    auto dataSize = std::make_shared<BUFFER_SIZE_TYPE>(message.size());
    writeBytes(BUFFER_MAX_SIZE, connectedSocket, (char*)dataSize.get());
    writeBytes(*dataSize, connectedSocket, message.c_str());
}

void WHost::writeBytes(unsigned int bytes, SOCKET socket, const char* msg) {
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

std::string WHost::receiveMessage(const Host* host) {
    //Currently, only one host can be connected at a time
    //Not working for multiple hosts

    auto bufferSize = std::shared_ptr<BUFFER_SIZE_TYPE>((BUFFER_SIZE_TYPE*)readBytes(BUFFER_MAX_SIZE, connectedSocket));
    auto buffer = std::shared_ptr<char>(readBytes(*bufferSize, connectedSocket));
    return buffer.get();
}

char* WHost::readBytes(unsigned int bytes, SOCKET socket) {
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

void WHost::closeSocket(SOCKET &connSocket) {
    int result = closesocket(connSocket);
    if(result == SOCKET_ERROR) {
        printError("Error closing socket");
    }
    connSocket = INVALID_SOCKET;
}

wchar_t* WHost::WSAGetLastErrorFormatted() {
    wchar_t *s = nullptr;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &s, 0, nullptr);
    return s;
}

void WHost::printError(const char* msg) {
    auto formattedError = WSAGetLastErrorFormatted();
    fprintf(stderr, "%s: %S\n", msg, formattedError);
    delete[](formattedError);
}