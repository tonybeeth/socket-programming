cmake_minimum_required(VERSION 3.5)
project(socket_programming)

set(CMAKE_CXX_STANDARD 11)

include_directories(rapidxml)
include_directories(Host)
include_directories(Client)
include_directories(Server)
include_directories(Common)

#
add_library(ServerLib Server/Server.h Server/Server.cpp)
#
add_library(ClientLib Client/Client.h Client/Client.cpp)
#
add_library(WClientLib Client/WClient.h Client/WClient.cpp)
target_link_libraries(WClientLib ClientLib ServerLib WS2_32)
#
add_library(PServerLib Server/PServer.h Server/PServer.cpp)
target_link_libraries(PServerLib ServerLib ClientLib)

add_definitions(-DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
                -DSHADERS_DIR=${PROJECT_SOURCE_DIR}/xml/)

add_executable(server ServerMain.cpp)
target_link_libraries(server PServerLib)

add_executable(client ClientMain.cpp)
target_link_libraries(client WClientLib)
