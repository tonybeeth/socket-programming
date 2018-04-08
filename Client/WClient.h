
#ifndef SOCKET_PROGRAMMING_WCLIENT_H
#define SOCKET_PROGRAMMING_WCLIENT_H

#include <Client.h>
#include <WHost.h>

class WClient : public WHost, public Client{
public:

    ~WClient();

    void connectServer(const std::string& ip, const std::string& port) override;

    void disconnectServer() override;

private:
    addrinfo* getServerInfo(const std::string& ip, const std::string& port);

    SOCKET createSocket(addrinfo* serverInfo);

    void connectSocket(SOCKET &connSocket, addrinfo* serverInfo);
};

#endif //SOCKET_PROGRAMMING_WCLIENT_H
