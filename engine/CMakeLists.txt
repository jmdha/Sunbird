set(ENGINE_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(ENGINE_INCLUDE_BUILD_DIR ${ENGINE_INCLUDE_DIR}/engine)

set(ENGINE_HEADERS
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/values.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/move_ordering.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/pv.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/tt.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/evaluation.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/search.hpp
    )

set(ENGINE_SRC
    ${ENGINE_SRC_DIR}/move_ordering.cpp
    ${ENGINE_SRC_DIR}/evaluation.cpp
    ${ENGINE_SRC_DIR}/search.cpp
    ${ENGINE_SRC_DIR}/search_internal.cpp
    ${ENGINE_SRC_DIR}/tt.cpp
    )

add_library(
    ENGINE 
    ${ENGINE_HEADERS}
    ${ENGINE_SRC}
)

target_link_libraries(
    ENGINE
    PUBLIC
    CHESS
)

target_include_directories(
    ENGINE
    PUBLIC
    "$<BUILD_INTERFACE:${ENGINE_INCLUDE_DIR}>"
)

target_compile_features(
    ENGINE
    PUBLIC
    cxx_std_20
)

include(${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt)
