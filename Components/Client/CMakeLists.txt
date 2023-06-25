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

add_executable(Client main.cpp inc/AuthorizationWindow.h inc/MainWindow.h inc/ConnectionWindow.h inc/ConnectionWindow.h inc/ConnectionWindow.h src/ConnectionWindow.cpp src/AuthorizationWindow.cpp src/MainWindow.cpp inc/NetworkClient.h src/NetworkClient.cpp)

include_directories(inc)
include_directories(${PROJECT_SOURCE_DIR}/Components/Library/inc)

target_link_libraries(Client
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Library
        )


