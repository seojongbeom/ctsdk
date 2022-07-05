cmake_minimum_required(VERSION 3.22)
project(resource_generator)

add_definitions(-D_GLIBCXX_USE_CXX11_ABI)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_definitions(
            -D_SCL_SECURE_NO_WARNINGS
            -D_CRT_SECURE_NO_WARNINGS
            -D_WIN32_WINNT=0x601
            -D_WINSOCK_DEPRECATED_NO_WARNINGS
    )
endif()

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(JsonCpp)

SET(SOURCE main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})
include_directories(${CMAKE_PREFIX_PATH}/include)
target_link_libraries(${PROJECT_NAME} JsonCpp::JsonCpp)
