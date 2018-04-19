#include <PServer.h>
#include <rapidxml_utils.hpp>
#include <Service.h>
#include <iostream>

int main(){
    rapidxml::file<> file("../xml/sample.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(file.data());
    auto hostNode = doc.first_node("Host");

    std::string ip = hostNode->first_attribute("IP")->value(), port = hostNode->first_attribute("PORT")->value();

    PServer server(hostNode->first_attribute("IP")->value(), hostNode->first_attribute("PORT")->value());

    server.setup();

    server.connectClient();

    printf("Connected\n");

    while(true) {
        auto packet = server.receiveMessage();
        if(packet->type == Packet::END)
            break;
        auto service = *((Service::ServiceType *) packet->data);

        if (service == Service::ADD) {
            printf("Add\n");
            auto packet = server.receiveMessage();
            auto vSize = (packet->dataSize) / sizeof(double);
            auto data = (double *) packet->data;
            std::vector<double> vdata(data, data + vSize);
            std::cout << "Received Values: \n";
            for (auto a : vdata) {
                std::cout << a << " ";
            }

            double result = Service::add(vdata);
            packet->dataSize = sizeof(double);
            packet->data = &result;
            server.sendMessage(packet);

            std::cout << "\n\nCalculated sum: " << result << std::endl;
        }
    }

    return 0;
}
