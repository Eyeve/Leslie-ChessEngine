cmake_minimum_required(VERSION 3.30)
project(Leslie_ChessEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} tests/tests.cpp)

include_directories(src)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
