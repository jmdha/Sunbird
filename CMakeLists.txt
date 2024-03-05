cmake_minimum_required(VERSION 3.14.0)

add_definitions(
        -D_NAME="Sunbird"
        -D_AUTHOR="jamadaha"
        -D_VERSION="dev"
)

project(
    ${_PROJECT_NAME}
    VERSION ${_PROJECT_VERSION}
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -march=native -flto)

set(sources
    src/bit.hpp
    src/bitboard.hpp
    src/board.hpp
    src/evaluation.hpp
    src/import.hpp
    src/move.hpp
    src/move_gen.hpp
    src/move_list.hpp
    src/move_ordering.hpp
    src/position.hpp
    src/pv.hpp
    src/search.hpp
    src/search_limit.hpp
    src/tt.hpp
    src/types.hpp
    src/utilities.hpp
    src/values.hpp
    src/zobrist.hpp
    src/bitboard.cpp
    src/board.cpp
    src/evaluation.cpp
    src/import_FEN.cpp
    src/import_sequence.cpp
    src/move.cpp
    src/move_gen.cpp
    src/move_ordering.cpp
    src/position.cpp
    src/search.cpp
    src/search_internal.cpp
    src/tt.cpp
    src/zobrist.cpp
)

include(bin/CMakeLists.txt)
include(tests/CMakeLists.txt)
