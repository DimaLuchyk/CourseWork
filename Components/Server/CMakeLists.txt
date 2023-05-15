cmake_minimum_required(VERSION 3.24)
project(Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Network
        REQUIRED)

add_executable(Server main.cpp Server.cpp)
target_link_libraries(Server
        Qt5::Core
        Qt5::Network
        )

