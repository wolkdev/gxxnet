cmake_minimum_required(VERSION 3.1)
project(gxxnet)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    set(SOCKET_SOURCE sources/Native/WinSocket.cpp)
else()
    set(SOCKET_SOURCE sources/Native/UnixSocket.cpp)
endif()

set(SOURCES
    ${SOCKET_SOURCE}
    sources/Packet.cpp
    sources/Test.cpp)

include_directories(includes)

add_executable(gxxnet ${SOURCES})

if(WIN32)
    target_link_libraries(gxxnet ws2_32)
endif()