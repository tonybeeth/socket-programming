
#ifndef SOCKET_PROGRAMMING_WCLIENT_H
#define SOCKET_PROGRAMMING_WCLIENT_H

#include <Client.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>

class WClient : public Client{
public:

    WClient(const std::string &ip, const std::string &port);

    WClient();

    virtual ~WClient();

    void connectServer(Server *);

    void sendMessage(const std::string &message, Server *server = nullptr);

    std::string receiveMessage();

    void disconnectServer();

private:
    addrinfo* getServerInfo(Server*);

    SOCKET createSocket(addrinfo* serverInfo);

    void connectSocket(SOCKET &connSocket, addrinfo* serverInfo);

    void closeSocket(SOCKET &connSocket);

    //Will keep reading from socket until all bytes are read
    char* receiveBytes(int bytes, SOCKET socket);

    //Will keep writing to socket until all bytes are written
    void writeBytes(int bytes, SOCKET socket, const char* msg);

    SOCKET connSocket;

    void fatalError(const std::string&);
};

#endif //SOCKET_PROGRAMMING_WCLIENT_H
