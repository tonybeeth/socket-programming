#include "Host.h"

Host::Host() : ip(""), port("") {}

Host::Host(const std::string &ip, const std::string &port) : ip(ip), port(port) {
}

const std::string& Host::getIp() const {
    return ip;
}

void Host::setIp(const std::string &ip) {
    Host::ip = ip;
}

const std::string& Host::getPort() const {
    return port;
}

void Host::setPort(const std::string &port) {
    Host::port = port;
}

std::string Host::toString() {
    return "IP: " + ip + ", PORT: " + port;
}
