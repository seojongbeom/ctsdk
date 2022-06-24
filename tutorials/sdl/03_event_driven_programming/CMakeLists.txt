cmake_minimum_required(VERSION 3.22)
project(03_event_driven_programming)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(blend2d)
FIND_PACKAGE(SDL2)

add_definitions(-DSDK_ROOT_PATH="${CMAKE_PREFIX_PATH}")

add_executable(03_event_driven_programming main.cpp)

target_link_libraries(${PROJECT_NAME} blend2d::blend2d SDL2::SDL2main SDL2::SDL2-static)