#include "WHost.h"
#include <memory>
#include <Packet.h>

WHost::WHost() : Host(), connectedSocket(INVALID_SOCKET) {}

WHost::WHost(const std::string &ip, const std::string &port) : Host(ip, port), connectedSocket(INVALID_SOCKET) {}

void WHost::sendMessage(const Packet* packet, const Host* host) {
    //Currently, only one host can be connected at a time
    //Not working for multiple hosts

    writeBytes(sizeof(Packet), connectedSocket, packet);
    writeBytes(packet->dataSize, connectedSocket, packet->data);
}

void WHost::writeBytes(unsigned int bytes, SOCKET socket, const void* msg) {
    int totalBytesWritten = 0, currentBytesWritten = 0;

    while(totalBytesWritten < bytes) {
        currentBytesWritten = send(socket, (char*)msg, bytes, 0);
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

Packet* WHost::receiveMessage(const Host* host) {
    //Currently, only one host can be connected at a time
    //Not working for multiple hosts

    auto packet = (Packet*)readBytes(sizeof(Packet), connectedSocket);
    packet->data = nullptr;
    if(packet->type != Packet::END) {
        packet->data = readBytes(packet->dataSize, connectedSocket);
    }

    return packet;
}

void* WHost::readBytes(unsigned int bytes, SOCKET socket) {
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