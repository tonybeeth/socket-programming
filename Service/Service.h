#ifndef SOCKET_PROGRAMMING_SERVICE_H
#define SOCKET_PROGRAMMING_SERVICE_H

#include<vector>

class Service {

public:

    enum ServiceType {ADD, MULTIPLY, SUBTRACT};

    static double add(std::vector<double>& numbers);

    static double multiple(std::vector<double>& numbers);

    static double subtract(std::vector<double>& numbers);
};


#endif //SOCKET_PROGRAMMING_SERVICE_H
