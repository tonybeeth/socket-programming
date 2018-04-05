
#ifndef SOCKET_PROGRAMMING_COMMON_H
#define SOCKET_PROGRAMMING_COMMON_H

#include <Host.h>
#include <rapidxml_utils.hpp>

Host getHost(std::string xmlFileName) {
    rapidxml::file<> file("../xml/sample.xml");
    rapidxml::xml_document<> doc;
    doc.parse<0>(file.data());
    auto hostNode = doc.first_node("Host");
    return Host(hostNode->first_attribute("IP")->value(), hostNode->first_attribute("PORT")->value());
}

#endif //SOCKET_PROGRAMMING_COMMON_H
