include(chess/CMakeLists.txt)

set(ENGINE_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(ENGINE_INCLUDE_BUILD_DIR ${ENGINE_INCLUDE_DIR}/engine)

set(ENGINE_HEADERS
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/values.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/positions.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/move_ordering.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/evaluation.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/negamax.hpp
    )

set(ENGINE_SRC
    ${ENGINE_SRC_DIR}/move_ordering.cpp
    ${ENGINE_SRC_DIR}/evaluation.cpp
    ${ENGINE_SRC_DIR}/negamax.cpp
    )

add_library(ENGINE ${ENGINE_HEADERS} ${ENGINE_SRC})
target_link_libraries(ENGINE PUBLIC CHESS)
target_include_directories(ENGINE PUBLIC ${ENGINE_INCLUDE_DIR})

if (NOT IGNORE_TEST)
    include(${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt)
endif ()

if (NOT IGNORE_BIN)
    add_executable(Sunbird ${CMAKE_CURRENT_LIST_DIR}/bin/uci.cpp)
    target_link_libraries(Sunbird PRIVATE ENGINE)
endif ()
