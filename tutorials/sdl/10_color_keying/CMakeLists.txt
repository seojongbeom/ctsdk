cmake_minimum_required(VERSION 3.22)
project(10_color_keying)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(blend2d)
FIND_PACKAGE(SDL2)
FIND_PACKAGE(SDL2_image)

add_definitions(-DSDK_ROOT_PATH="${CMAKE_PREFIX_PATH}")
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${blend2d_LIBRARIES} ${SDL2_LIBRARIES}-static SDL2_image::SDL2_image-static)