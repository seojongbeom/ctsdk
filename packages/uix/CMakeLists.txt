cmake_minimum_required(VERSION 3.22)
project(uix)

set(CMAKE_CXX_STANDARD 17)

add_library(uix uix.cpp uix_core_enums.h)
