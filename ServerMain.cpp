#include <PServer.h>
#include <rapidxml_utils.hpp>

int main(){

    rapidxml::file<> file("../xml/sample.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(file.data());
    auto hostNode = doc.first_node("Host");

    std::string ip = hostNode->first_attribute("IP")->value(), port = hostNode->first_attribute("PORT")->value();

    PServer server(hostNode->first_attribute("IP")->value(), hostNode->first_attribute("PORT")->value());

    server.setup();

    server.connectClient();

    std::string msg = server.receiveMessage();

    printf("%s\n", msg.c_str());

    server.sendMessage("I got what you sent!!");



    return 0;
}
