#include "PHost.h"
#include <memory>
#include <unistd.h>

PHost::PHost() : Host(), connectedSocket(-1) {}

PHost::PHost(const std::string &ip, const std::string &port) : Host(ip, port), connectedSocket(-1) {
}

void PHost::sendMessage(const Packet* packet, const Host *host) {
    //Write size
    writeBytes(sizeof(Packet), connectedSocket, packet);
    writeBytes(packet->dataSize, connectedSocket, packet->data);
}

Packet* PHost::receiveMessage(const Host *host) {
    auto packet = (Packet*)readBytes(sizeof(Packet), connectedSocket);
    packet->data = nullptr;
    if(packet->type != Packet::END) {
        packet->data = readBytes(packet->dataSize, connectedSocket);
    }

    return packet;
}

void* PHost::readBytes(unsigned int bytes, int socket) {
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

void PHost::writeBytes(unsigned int bytes, int socket, const void* msg) {
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

void PHost::printError(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void PHost::closeSocket(int &socket) {
    if (close(socket) != 0) {
        printError("Error closing socket");
    }
}
