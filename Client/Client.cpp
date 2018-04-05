
#include "Client.h"

Client::Client() : Host() {}

Client::Client(const std::string &ip, const std::string &port) : Host(ip, port) {}
