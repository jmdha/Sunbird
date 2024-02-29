cmake_minimum_required(VERSION 3.14.0)

add_definitions(
        -D_NAME="Sunbird"
        -D_AUTHOR="jamadaha"
        -D_VERSION="dev"
)

project(
    ${_NAME}
    VERSION ${_VERSION}
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -march=native -flto)

set(sources
    src/bb.hpp
    src/board.hpp
    src/evaluation.hpp
    src/masks.hpp
    src/move.hpp
    src/move_gen.hpp
    src/move_ordering.hpp
    src/search.hpp
    src/tt.hpp
    src/types.hpp
    src/values.hpp
    src/zobrist.hpp
    src/board.cpp
    src/evaluation.cpp
    src/masks.cpp
    src/move.cpp
    src/move_gen.cpp
    src/move_ordering.cpp
    src/search.cpp
    src/tt.cpp
    src/zobrist.cpp
)

include(bin/CMakeLists.txt)
include(tests/CMakeLists.txt)
