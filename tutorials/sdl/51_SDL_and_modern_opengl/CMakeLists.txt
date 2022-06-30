cmake_minimum_required(VERSION 3.22)
project(51_SDL_and_modern_opengl)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(blend2d)
FIND_PACKAGE(SDL2)
FIND_PACKAGE(SDL2_image)
FIND_PACKAGE(Freetype)
FIND_PACKAGE(PNG)
FIND_PACKAGE(harfbuzz)
FIND_PACKAGE(ZLIB)
FIND_PACKAGE(OpenGL)

include_directories(${SDL2_ttf_SOURCE_DIR})
add_definitions(-DSDK_ROOT_PATH="${CMAKE_PREFIX_PATH}")
add_executable(${PROJECT_NAME} main.cpp)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

target_link_libraries(${PROJECT_NAME} ${blend2d_LIBRARIES} ${SDL2_LIBRARIES}-static)
target_link_libraries(${PROJECT_NAME} SDL2_image::SDL2_image-static SDL2_ttf)
target_link_libraries(${PROJECT_NAME} ${Freetype_LIBRARIES} ${harfbuzz_LIBRARIES} ${PNG_LIBRARIES} ${ZLIB_LIBRARIES} ${OPENGL_LIBRARIES} ${CMAKE_PREFIX_PATH}/lib/glew32.lib)