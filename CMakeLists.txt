cmake_minimum_required(VERSION 3.27.8)
project(Leslie_ChessEngine)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} tests/tests.cpp
        src/position.cpp
        src/lichess_shell.cpp
        src/engine.cpp
        src/move.cpp
        src/options.cpp
        tests/knight_test.cpp
        tests/king_test.cpp
        tests/pawn_test.cpp
        tests/advanced_test.cpp
        src/board.cpp
        src/pieces_container.cpp
        src/pieces_container.h
        tests/rook_test.cpp
        tests/bishop_test.cpp
        tests/tests.h
)

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


enable_testing()
add_test(NAME LeslieTests COMMAND ${PROJECT_NAME})
