cmake_minimum_required(VERSION 3.10)
project(TriangulationSDL2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

if(WIN32)
    add_compile_options(-mwindows)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()

find_package(SDL2 REQUIRED)

set(SDL2_GFX_INCLUDE_DIR "C:\\msys64\\mingw64\\include\\SDL2")
set(SDL2_GFX_LIBRARY "C:\\msys64\\mingw64\\lib\\libSDL2_gfx.dll.a")

set(SRC_LIST sources/main.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

add_library(primitives STATIC sources/primitives.cpp)
target_include_directories(primitives PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES}
    ${SDL2_GFX_LIBRARY}
    primitives
)