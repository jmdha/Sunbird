cmake_minimum_required(VERSION 3.0.0)
project(chess_engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare (
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG master
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
set(TRACY_ENABLE ON CACHE BOOL "Enable profiling")
#set(TRACY_NO_SYSTEM_TRACING ON CACHE BOOL "Disable System Tracing")
#set(TRACY_ONLY_IPV4 ON CACHE BOOL "IPv4 only")
option(TRACY_ENABLE "Enable profiling" ON)
#option(TRACY_NO_SYSTEM_TRACING "Disable System Tracing" ON)
#option(TRACY_ONLY_IPV4 "IPv4 only" ON)
message("Fetching tracy")
FetchContent_MakeAvailable(tracy)

include(CTest)
enable_testing()

set(sources
        src/bit_shifts.cpp src/board.cpp src/board_importer.cpp src/evaluator.cpp src/minimax.cpp src/headers/openings.hh
        src/move_gen/move_gen.cpp src/move_gen/piece_gen.cpp src/move_gen/king_gen.cpp src/move_gen/knight_gen.cpp src/move_gen/pawn_gen.cpp
        include/robin-map/robin_growth_policy.h include/robin-map/robin_hash.h include/robin-map/robin_map.h include/robin-map/robin_set.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} TRACY_ENABLE")
add_executable(main builds/main.cpp ${sources})
add_executable(chess_engine builds/engine.cpp ${sources})
add_executable(chess_engine_fen builds/engine_fen.cpp ${sources})
add_executable(perft builds/perft.cpp ${sources} "src/perft.cpp" "include/tracy/TracyClient.cpp")
add_executable(tune builds/tune.cpp ${sources} src/headers/positions.hh "include/indicators/indicators.hpp")

include_directories("src" "src/headers" "src/move_gen" "src/move_gen/headers")
target_include_directories(tune PRIVATE "include")

target_link_libraries(perft PUBLIC TracyClient pthread dl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
include("tests/CMakeLists.txt")