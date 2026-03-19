cmake_minimum_required(VERSION 3.27.8)
project(Leslie_ChessEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(leslie_core
  src/engine.cpp
  src/move.cpp
  src/move_table.cpp
  src/pieces_container.cpp
  src/position.cpp
  src/thread.cpp
  src/utility.cpp
  src/nnue/nnue.cpp
)

target_include_directories(leslie_core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/Eigen
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(leslie_core PRIVATE -mbmi2)
endif()

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/lichess_shell.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE leslie_core)

add_executable(LeslieTests
  tests/tests.cpp
  tests/advanced_test.cpp
  tests/bishop_test.cpp
  tests/king_test.cpp
  tests/knight_test.cpp
  tests/nnue_test.cpp
  tests/pawn_test.cpp
  tests/rook_test.cpp
)
target_link_libraries(LeslieTests PRIVATE leslie_core gtest gtest_main)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(LeslieTests PRIVATE -mbmi2)
endif()

enable_testing()
add_test(NAME LeslieTests COMMAND LeslieTests)
