#ifndef SOCKET_PROGRAMMING_SERVICE_H
#define SOCKET_PROGRAMMING_SERVICE_H

#include <vector>
#include <string>

class Service {

public:

    enum ServiceType {INVALID, ADD, MULTIPLY, SUBTRACT, STOCK_INFO};

    static double add(std::vector<double>& numbers);

    static double multiply(std::vector<double> &numbers);

    static double subtract(std::vector<double>& numbers);

    static std::string getStockInfo(const std::string& stockSymbol);
};


#endif //SOCKET_PROGRAMMING_SERVICE_H
