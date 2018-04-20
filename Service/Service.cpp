#include "Service.h"
#include <numeric>
#include <functional>
#include <AvapiConnection.hpp>
#include <TIME_SERIES/TIME_SERIES_DAILY.hpp>
#include <sstream>

double Service::add(std::vector<double>& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 0.0);
}

double Service::multiply(std::vector<double> &numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 1.0, std::multiplies<double>());
}

double Service::subtract(std::vector<double>& numbers) {
    return std::accumulate(numbers.begin(), numbers.end(), 0.0, std::minus<double>());
}

std::string Service::getStockInfo(const std::string &stockSymbol) {
    std::string lastHttpRequest = "";

    Avapi::AvapiConnection* avapi_connection;
    try {
        avapi_connection = Avapi::AvapiConnection::getInstance();
        avapi_connection->set_ApiKey("KMC29PVU5AZFWAIK");
    }
    catch(Avapi::AvapiConnectionError& e) {
        fprintf(stderr, "AvapiConnectionError: %s\n", e.get_error().c_str());
        exit(EXIT_FAILURE);
    }

    auto& queryObject = avapi_connection->GetQueryObject_TIME_SERIES_DAILY();
    auto response = queryObject.Query(stockSymbol, Avapi::Const_TIME_SERIES_DAILY_outputsize::compact,
                                      Avapi::Const_TIME_SERIES_DAILY_datatype::json);
    if(response.get_Data().isError()) {
        fprintf(stderr, "Avapi Query Response Error: %s\n", response.get_Data().get_ErrorMessage().c_str());
        exit(EXIT_FAILURE);
    }
    else {
        auto& metaData  = response.get_Data().get_MetaData();
        auto& timeSeries = response.get_Data().get_TimeSeries();

        std::stringstream ss;

        ss << "========================\n";
        ss << "Information: " <<  metaData.get_Information() << "\n";
        ss << "Symbol: " <<  metaData.get_Symbol() << "\n";
        ss << "TimeZone: " <<  metaData.get_TimeZone() << "\n";
        ss << "========================" << "\n";

        auto firstElement = timeSeries.begin();
        ss << "Open: " << firstElement->get_Open() << "\n";
        ss << "High: " << firstElement->get_High() << "\n";
        ss << "Low: " << firstElement->get_Low() << "\n";
        ss << "Close: " << firstElement->get_Close() << "\n";
        ss << "Volume: " << firstElement->get_Volume() << "\n";
        ss << "DateTime: " << firstElement->get_DateTime() << "\n";
        ss << "========================" << "\n";

        std::string stockInfo;

        ss.seekg(0, std::ios::end);
        stockInfo.reserve(ss.tellg());
        ss.seekg(0, std::ios::beg);

        stockInfo.assign((std::istreambuf_iterator<char>(ss)),
                   std::istreambuf_iterator<char>());

        return stockInfo;
    }
}
