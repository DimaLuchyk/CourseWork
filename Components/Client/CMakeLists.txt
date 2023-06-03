cmake_minimum_required(VERSION 3.24)
project(Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(Client main.cpp src/Client.cpp inc/AuthorisationWindow.h src/AuthorisationWindow.cpp)

include_directories(inc)

target_link_libraries(Client
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        )

