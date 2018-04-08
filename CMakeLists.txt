cmake_minimum_required(VERSION 3.5)
project(socket_programming)

set(CMAKE_CXX_STANDARD 11)

include_directories(rapidxml)
include_directories(Host)
include_directories(Client)
include_directories(Server)
#include_directories(Common)

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
target_link_libraries(server PServerLib)

add_executable(client ClientMain.cpp)
target_link_libraries(client WClientLib)
