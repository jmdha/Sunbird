cmake_minimum_required(VERSION 3.14.0)
project(chess_engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

if (BUILD_ENGINE)
    add_subdirectory(engine)
else ()
    add_subdirectory(chess)
endif ()

