cmake_minimum_required (VERSION 2.6)
project (gxxnet)

add_executable(gxxnet Test.cpp Socket.cpp)

if (WIN32)
target_link_libraries(gxxnet ws2_32)
endif()