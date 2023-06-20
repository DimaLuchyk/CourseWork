cmake_minimum_required(VERSION 3.24)
project(Library)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Network
        Sql
        REQUIRED)

set(SOURCES src/PacketGenerator.cpp)
set(HEADERS inc/PacketGenerator.h inc/Packet.h inc/Serializer.h src/Serializer.cpp)

include_directories(inc)

# Create the shared library
add_library(Library SHARED ${SOURCES} ${HEADERS})

# Set the library include directories
target_include_directories(Library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(Library
        Qt5::Core
        Qt5::Network
        Qt5::Sql
        )
