cmake_minimum_required(VERSION 3.22)
project(SDL_02_getting_an_image_on_the_screen)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

FIND_PACKAGE(blend2d)
FIND_PACKAGE(SDL2)

add_definitions(-DSDK_ROOT_PATH="${CMAKE_PREFIX_PATH}")
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${blend2d_LIBRARIES} ${SDL2_LIBRARIES}-static )