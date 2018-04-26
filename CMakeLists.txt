cmake_minimum_required(VERSION 3.5)
project(socket_programming)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Vendor/selene)
include_directories(Vendor/selene/src)

include_directories(rapidxml)
include_directories(Host)
include_directories(Client)
include_directories(Server)
include_directories(Service)
include_directories(Packet)

set(AvapiDir Vendor/AvapiCpp/Linux/Ubuntu/64bit/AVAPI/Avapi)
include_directories(${AvapiDir})
#find_library(AvapiLib avapi PATHS ${AvapiDir})
#if(NOT AvapiLib)
#    message(FATAL_ERROR "AvapiLib library not found")
#endif()

add_library(ServiceLib Service/Service.h Service/Service.cpp)
link_directories(${AvapiDir}) #Specify directory to search for avapi library file
target_link_libraries(ServiceLib avapi selene)
#
add_library(HostLib Host/Host.h Host/Host.cpp)
#
add_library(WHostLib Host/WHost.h Host/WHost.cpp)
target_link_libraries(WHostLib HostLib WS2_32)
#
add_library(PHostLib Host/PHost.h Host/PHost.cpp)
target_link_libraries(PHostLib HostLib)
#
add_library(ServerLib Server/Server.h Server/Server.cpp)
target_link_libraries(ServerLib HostLib)
#
add_library(ClientLib Client/Client.h Client/Client.cpp)
target_link_libraries(ClientLib HostLib)
#
add_library(WClientLib Client/WClient.h Client/WClient.cpp)
target_link_libraries(WClientLib ServerLib ClientLib WHostLib WS2_32)
#
add_library(PServerLib Server/PServer.h Server/PServer.cpp)
target_link_libraries(PServerLib ServerLib ClientLib PHostLib)


add_definitions(-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
                -DSHADERS_DIR=${PROJECT_SOURCE_DIR}/xml/)

add_executable(server ServerMain.cpp)
target_link_libraries(server PServerLib ServiceLib)

add_executable(client ClientMain.cpp)
target_link_libraries(client WClientLib)
