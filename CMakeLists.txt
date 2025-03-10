cmake_minimum_required(VERSION 3.30)
project(Leslie_ChessEngine)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Leslie_ChessEngine
    src/engine.cpp
    src/engine.h)
