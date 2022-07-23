cmake_minimum_required(VERSION 3.0.0)
project(chess_engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

file(GLOB_RECURSE sources CONFIGURE_DEPENDS classes/*.cpp classes/*.h)

add_executable(main builds/main.cpp ${sources})
add_executable(test_run builds/test_run.cpp ${sources})
add_executable(chess_engine builds/engine.cpp ${sources})
add_executable(chess_engine_fen builds/engine_fen.cpp ${sources})
add_executable(move_tree builds/move_tree.cpp ${sources})
add_executable(testBuild builds/test.cpp ${sources})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
include(tests/CMakeTests.txt)