cmake_minimum_required (VERSION 3.1)
project (gxxnet)

set(CMAKE_CXX_STANDARD 11)

add_executable(gxxnet Test.cpp Socket.cpp)

if (WIN32)
target_link_libraries(gxxnet ws2_32)
endif()