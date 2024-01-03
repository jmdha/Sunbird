set(CHESS_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(CHESS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(CHESS_INCLUDE_BUILD_DIR ${CHESS_INCLUDE_DIR}/chess)

set(CHESS_HEADERS
    ${CHESS_INCLUDE_BUILD_DIR}/internal/bit.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/bitboard.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/constants.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/move.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/move_list.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/utilities.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/position.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/zobrist.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/board.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/import.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/export.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/move_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/perft.hpp
    )

set(CHESS_SRC
    ${CHESS_SRC_DIR}/board.cpp
    ${CHESS_SRC_DIR}/bitboard.cpp
    ${CHESS_SRC_DIR}/move_gen.cpp
    ${CHESS_SRC_DIR}/perft.cpp
    ${CHESS_SRC_DIR}/import_FEN.cpp
    ${CHESS_SRC_DIR}/import_sequence.cpp
    ${CHESS_SRC_DIR}/export_FEN.cpp
    ${CHESS_SRC_DIR}/position.cpp
    ${CHESS_SRC_DIR}/zobrist.cpp
    )

add_library(
    CHESS
    ${CHESS_HEADERS}
    ${CHESS_SRC}
)

target_include_directories(
    CHESS
    PUBLIC
    "$<BUILD_INTERFACE:${CHESS_INCLUDE_DIR}>"
)

target_compile_features(
    CHESS
    PUBLIC
    cxx_std_20
)

if (NOT IGNORE_TEST)
    include(${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt)
endif ()
