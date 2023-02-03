cmake_minimum_required(VERSION 3.0.0)
project(chess_engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

set(sources
        src/bit_shifts.cpp src/board.cpp src/board_importer.cpp src/evaluator.cpp src/minimax.cpp src/headers/openings.hh
        src/move_gen/move_gen.cpp src/move_gen/piece_gen.cpp src/move_gen/king_gen.cpp src/move_gen/knight_gen.cpp src/move_gen/pawn_gen.cpp)

add_executable(main builds/main.cpp ${sources})
add_executable(chess_engine builds/engine.cpp ${sources})
add_executable(perft builds/perft.cpp ${sources} "src/perft.cpp")
add_executable(tune builds/tune.cpp ${sources} src/headers/positions.hh "include/indicators/indicators.hpp")
add_executable(test_feature builds/test_feature.cpp ${sources} src/headers/positions.hh "include/indicators/indicators.hpp")

include_directories("src" "src/headers" "src/move_gen" "src/move_gen/headers")
target_include_directories(tune PRIVATE "include")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
include("tests/CMakeLists.txt")