#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <WClient.h>
#include <Service.h>
#include <Packet.h>
#include <memory>
#include <numeric>

void InitWinsock();

Service::ServiceType getServiceOption();

std::vector<double> getNumbers();

std::string getStockSymbol();

int main() {
    rapidxml::file<> file("../xml/sample.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(file.data());
    auto host = doc.first_node("Host");

    //Initialize Winsock
    InitWinsock();

    WClient client;
    std::string serverIP = host->first_attribute("IP")->value();
    std::string serverPort = host->first_attribute("PORT")->value();

    client.connectServer(serverIP, serverPort);
    printf("\nConnected to Server. IP: %s, Port: %s\n", serverIP.c_str(), serverPort.c_str());

    //Create service and data packets
    auto servicePacket = new Packet;
    auto dataPacket = new Packet;
    servicePacket->type = dataPacket->type = Packet::DATA;
    servicePacket->dataSize = sizeof(Service::ServiceType);

    Service::ServiceType serviceOption;

    while((serviceOption = getServiceOption()) != Service::INVALID) {
        //Set service option and send service packet
        servicePacket->data = &serviceOption;
        client.sendMessage(servicePacket);
        Packet* recvPacket;

        switch (serviceOption) {
            case Service::INVALID:
                break;
            case Service::ADD:
            case Service::SUBTRACT:
            case Service::MULTIPLY: {
                auto numbers = getNumbers();
                dataPacket->dataSize = numbers.size() * sizeof(double);
                dataPacket->count = numbers.size();
                dataPacket->data = numbers.data();
                client.sendMessage(dataPacket);
                recvPacket = client.receiveMessage();
                printf("Returned Value: %f\n", *((double *) recvPacket->data));
                break;
            }
            case Service::STOCK_INFO: {
                auto stockSymbol = getStockSymbol();
                /*dataPacket->dataSize = std::accumulate(stockSymbols.begin(), stockSymbols.end(), (unsigned int)0,
                    [](int sum, const std::string& str){ return sum + sizeof(std::string) + str.length(); });*/
                dataPacket->dataSize = stockSymbol.length();
                dataPacket->data = (void*) stockSymbol.data();
                dataPacket->count = stockSymbol.length();
                client.sendMessage(dataPacket);
                recvPacket = client.receiveMessage();
                //printf("Returned Value: %f\n", *((double*)recvPacket->data));
                printf("Stock Information:\n%s\n", recvPacket->data);
                break;
            }
        }
    }

    client.disconnectServer();

    WSACleanup();

    return 0;
}

std::string getStockSymbol() {
    printf("Enter Stock symbol: ");
    std::string symbol;
    std::cin >> symbol;
    return symbol;
}

std::vector<std::string> getStockSymbols() {
    printf("Enter Stock symbols separated with spaces. Hit Enter to stop: ");
    std::vector<std::string> symbols;
    std::string symbol;
    std::cin.get();
    while(std::cin.peek() != '\n') {
        std::cin >> symbol;
        symbols.push_back(symbol);
    }
    return symbols;
}

std::vector<double> getNumbers() {
    printf("Enter numbers separated with spaces. Hit Enter to stop: ");
    std::vector<double> numbers;
    double val;
    std::cin.get();
    while(std::cin.peek() != '\n'){
        std::cin >> val;
        numbers.push_back(val);
    }
    return numbers;
}

Service::ServiceType getServiceOption() {
    printf("Service Options:\n");
    printf("1 - Sum Numbers\n");
    printf("2 - Subtract Numbers\n");
    printf("3 - Multiply Numbers\n");
    printf("4 - Get Stock Info\n");
    printf("0 - Quit\n");

    std::vector<Service::ServiceType> serviceTypes =
            {Service::INVALID, Service::ADD, Service::SUBTRACT, Service::MULTIPLY,
             Service::STOCK_INFO};

    int option = serviceTypes.size() + 1;
    for (auto i = 0; option < 0 or option > serviceTypes.size(); ++i) {
        if(i > 0) {
            printf("\nInvalid option entered\nEnter Service Option:");
        }
        std::cin >> option;
    }

    return serviceTypes[option];
}


void InitWinsock() {
    WSADATA wsadata;
    int result = WSAStartup(MAKEWORD(2,2), &wsadata);

    if(result != 0) {
        printf("WSAStartup failed: %d\n", result);
        exit(EXIT_FAILURE);
    }
}