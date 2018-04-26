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
		printf("\n");
        auto servicePacket = server.receiveMessage();
        if(servicePacket->type == Packet::END) {
            break;
        }
        auto service = *((Service::ServiceType *) servicePacket->data);

        auto recvPacket = server.receiveMessage();
        auto dataPacket = new Packet;

        if (service == Service::ADD || service == Service::SUBTRACT || service == Service::MULTIPLY) {
            auto data = (double*) recvPacket->data;
            std::vector<double> vdata(data, data + recvPacket->count);
            printf("Received Values: ");
            for (auto &a : vdata) {
                std::cout << a << " ";
            }

            double result;
            if(service == Service::ADD) {
                result = Service::add(vdata);
                printf("Addition\n");
            }
            else if(service == Service::SUBTRACT) {
                result = Service::subtract(vdata);
                printf("Subtraction\n");
            }
            else {
                result = Service::multiply(vdata);
                printf("Multiplication\n");
            }

            printf("Calculated Result: %f\n", result);
            dataPacket->dataSize = sizeof(double);
            dataPacket->data = &result;
            server.sendMessage(dataPacket);
        }
        else if (service == Service::STOCK_INFO) {
            printf("Stock Info\n");
            std::string stockSymbol((char*)recvPacket->data, recvPacket->dataSize);
            printf("Stock symbol:\n%s\n", stockSymbol.c_str());
            auto stockInfo = Service::getStockInfo(stockSymbol);
            printf("Stock Information: %s\n", stockInfo.c_str());
            dataPacket->data = (char*)stockInfo.c_str();
            dataPacket->dataSize = stockInfo.length();
            dataPacket->count = 1;
            server.sendMessage(dataPacket);
        }
        else { //Grayscale Image
			printf("Converting Image to Grayscale: %f KBs.\n", recvPacket->dataSize / 1024.0);
            auto grayImageVector = Service::convertImageToGrayScale(recvPacket->data, recvPacket->dataSize);
            dataPacket->data = grayImageVector.data();
            dataPacket->dataSize = grayImageVector.size();
            dataPacket->count = 1;
            server.sendMessage(dataPacket);
            printf("Image Converted.\n");
        }

		delete servicePacket;
		delete recvPacket;
		delete dataPacket;
    }

    return 0;
}
