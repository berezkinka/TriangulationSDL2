cmake_minimum_required(VERSION 3.10)
project(TriangulationSDL2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SRC_LIST sources/main.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

