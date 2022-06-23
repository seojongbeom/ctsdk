cmake_minimum_required(VERSION 3.22)
project(hello_sdl_with_blend2d)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(blend2d)
FIND_PACKAGE(SDL2)

add_definitions(-DSDK_ROOT_PATH="${CMAKE_PREFIX_PATH}")

add_executable(hello_sdl_with_blend2d hello_sdl_with_blend2d.cpp)


target_link_libraries(hello_sdl_with_blend2d blend2d::blend2d SDL2::SDL2main SDL2::SDL2-static)