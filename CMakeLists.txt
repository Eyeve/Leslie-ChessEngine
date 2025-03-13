cmake_minimum_required(VERSION 3.27.8)
project(Leslie_ChessEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} tests/tests.cpp
        src/position.cpp
        src/lichess_shell.cpp
        src/engine.cpp
        src/move.cpp
        src/options.cpp
        tests/knight_move_tests.cpp
        tests/king_move_test.cpp
        tests/pawn_move_tests.cpp
        tests/KNP_move_test.cpp
        tests/rays_test.cpp)

include_directories(src)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
        DOWNLOAD_EXTRACT_TIMESTAMP true
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mbmi2")

FetchContent_MakeAvailable(googletest)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
