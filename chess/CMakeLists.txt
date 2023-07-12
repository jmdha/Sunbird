set(CHESS_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(CHESS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(CHESS_INCLUDE_BUILD_DIR ${CHESS_INCLUDE_DIR}/chess)

set(CHESS_HEADERS
    ${CHESS_INCLUDE_BUILD_DIR}/internal/bit_shifts.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/constants.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/utilities.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/zobrist.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/piece_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/pawn_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/knight_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/bishop_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/rook_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/queen_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/internal/king_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/board.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/import.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/move.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/move_gen.hpp
    ${CHESS_INCLUDE_BUILD_DIR}/perft.hpp
    )

set(CHESS_SRC
    ${CHESS_SRC_DIR}/bit_shifts.cpp
    ${CHESS_SRC_DIR}/board.cpp
    ${CHESS_SRC_DIR}/perft.cpp
    ${CHESS_SRC_DIR}/move_gen/move_gen.cpp
    ${CHESS_SRC_DIR}/move_gen/piece_gen.cpp
    ${CHESS_SRC_DIR}/move_gen/pawn_gen.cpp
    ${CHESS_SRC_DIR}/move_gen/king_gen.cpp
    )

add_library(CHESS ${CHESS_HEADERS} ${CHESS_SRC})
set_target_properties(CHESS PROPERTIES LINKER_LANGUAGE CXX)

include_directories(${CHESS_INCLUDE_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt)

