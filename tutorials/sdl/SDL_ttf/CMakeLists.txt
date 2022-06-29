cmake_minimum_required(VERSION 3.22)
project(SDL2_ttf)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(blend2d)
FIND_PACKAGE(SDL2)
FIND_PACKAGE(SDL2_image)
FIND_PACKAGE(Freetype)
FIND_PACKAGE(PNG)
FIND_PACKAGE(harfbuzz)

SET(CMAKE_LINK_LIBRARY_FLAG "${CMAKE_LINK_LIBRARY_FLAG} -static")

add_definitions(-DSDK_ROOT_PATH="${CMAKE_PREFIX_PATH}")

add_library(${PROJECT_NAME} ../../../packages/SDL_ttf/SDL_ttf.h ../../../packages/SDL_ttf/SDL_ttf.c)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static SDL2_image::SDL2_image-static Freetype::Freetype ${PNG_LIBRARIES}  ${ZLIB_LIBRARIES} harfbuzz::harfbuzz)