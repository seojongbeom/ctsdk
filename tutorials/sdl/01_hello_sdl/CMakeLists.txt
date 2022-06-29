cmake_minimum_required(VERSION 3.22)
project(01_hello_sdl)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(blend2d)
FIND_PACKAGE(SDL2)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} blend2d::blend2d SDL2::SDL2main SDL2::SDL2-static)