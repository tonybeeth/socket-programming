#include "Service.h"
#include <numeric>
#include <functional>

double Service::add(std::vector<double>& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 0.0);
}

double Service::multiple(std::vector<double>& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 1.0, std::multiplies<double>());
}

double Service::subtract(std::vector<double>& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 0.0, std::minus<double>());
}
