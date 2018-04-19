#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <WClient.h>
#include <Service.h>
#include <Packet.h>

void InitWinsock() {
    WSADATA wsadata;
    int result = WSAStartup(MAKEWORD(2,2), &wsadata);

    if(result != 0) {
        printf("WSAStartup failed: %d\n", result);
        exit(EXIT_FAILURE);
    }
}

int main() {
    rapidxml::file<> file("../xml/sample.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(file.data());
    auto host = doc.first_node("Host");

    //Initialize Winsock
    InitWinsock();

    WClient client;

    client.connectServer(host->first_attribute("IP")->value(), host->first_attribute("PORT")->value());

    //std::string msg = "This is a message from the Client";

    std::vector<double> v = {1, 2, 3, 4, 1, 4};

    auto service = Service::ADD;
    Packet* packet = new Packet;
    packet->dataSize = sizeof(Service::ServiceType);
    packet->data = &service;
    packet->type = Packet::DATA;

    client.sendMessage(packet);
    packet->dataSize = v.size() * sizeof(double);
    packet->data = v.data();

    client.sendMessage(packet);

    auto result = client.receiveMessage();

    std::cout << "Sum is: " << *((double*)result->data) << std::endl;

    client.disconnectServer();

    WSACleanup();

    return 0;
}