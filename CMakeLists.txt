cmake_minimum_required(VERSION 3.14.0)
project(chess_engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_LIST_DIR}/chess/include)

if (BUILD_ENGINE)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/engine/include)
    add_subdirectory(engine)
else ()
    add_subdirectory(chess)
endif ()

