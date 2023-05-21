cmake_minimum_required(VERSION 3.24)
project(Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Network
        Sql
        REQUIRED)

add_executable(Server main.cpp src/Server.cpp src/DatabaseController.cpp inc/DatabaseController.h inc/Shared.h inc/PacketProcessor/PacketProcessor.h src/PacketProcessor/PacketProcessor.cpp)

include_directories(inc)
include_directories(../../3rdparty/spdlog/include)

target_link_libraries(Server
        Qt5::Core
        Qt5::Network
        Qt5::Sql
        )

