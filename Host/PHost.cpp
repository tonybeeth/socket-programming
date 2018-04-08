
#include "PHost.h"
#include <memory>
#include <unistd.h>

PHost::PHost() : Host(), connectedSocket(-1) {}

PHost::PHost(const std::string &ip, const std::string &port) : Host(ip, port), connectedSocket(-1) {
}

void PHost::sendMessage(const std::string &message, const Host *host) {
    auto dataSize = std::shared_ptr<BUFFER_SIZE_TYPE>(new BUFFER_SIZE_TYPE(message.size()));
    //Write size
    writeBytes(BUFFER_MAX_SIZE, connectedSocket, (char*)dataSize.get());
    writeBytes(*dataSize, connectedSocket, message.c_str());
}

std::string PHost::receiveMessage(const Host *host) {
    auto bufferSize = std::shared_ptr<BUFFER_SIZE_TYPE>((BUFFER_SIZE_TYPE*) readBytes(BUFFER_MAX_SIZE, connectedSocket));
    auto buffer = std::shared_ptr<char>(readBytes(*bufferSize, connectedSocket));
    return buffer.get();
}
char* PHost::readBytes(unsigned int bytes, int socket) {
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

void PHost::writeBytes(unsigned int bytes, int socket, const char* msg) {
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
