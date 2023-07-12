include(../chess/CMakeLists.txt)

set(ENGINE_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(ENGINE_INCLUDE_BUILD_DIR ${ENGINE_INCLUDE_DIR}/engine)

set(ENGINE_HEADERS
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/openings.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/positions.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/evaluator.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/minimax.hpp
    )

set(ENGINE_SRC
    ${ENGINE_SRC_DIR}/evaluator.cpp
    ${ENGINE_SRC_DIR}/minimax.cpp
    )

include_directories(${CHESS_INCLUDE_DIR} ${ENGINE_INCLUDE_DIR})
add_library(ENGINE ${ENGINE_HEADERS} ${ENGINE_SRC})
target_link_libraries(ENGINE PUBLIC CHESS)

add_executable(engine ${CMAKE_CURRENT_LIST_DIR}/bin/engine.cpp)
target_link_libraries(engine PRIVATE ENGINE)
add_executable(tune ${CMAKE_CURRENT_LIST_DIR}/bin/tune.cpp)
target_link_libraries(tune PRIVATE ENGINE)
add_executable(perft ${CMAKE_CURRENT_LIST_DIR}/bin/perft.cpp)
target_link_libraries(perft PRIVATE ENGINE)

include(${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt)

