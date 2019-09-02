cmake_minimum_required (VERSION 3.1)
project (gxxnet)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
    sources/Test.cpp)

include_directories(includes)

add_executable(gxxnet ${SOURCES})

if (WIN32)
target_link_libraries(gxxnet ws2_32)
endif()