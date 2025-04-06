cmake_minimum_required(VERSION 3.10)
project(TriangulationSDL2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package(SDL2 REQUIRED)

set(SDL2_GFX_INCLUDE_DIR "C:\\msys64\\mingw64\\include\\SDL2")
set(SDL2_GFX_LIBRARY "C:\\msys64\\mingw64\\lib\\libSDL2_gfx.dll.a")

set(SRC_LIST sources/main.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES}
    ${SDL2_GFX_LIBRARY}
)