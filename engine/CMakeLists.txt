include(../chess/CMakeLists.txt)

set(ENGINE_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(ENGINE_INCLUDE_BUILD_DIR ${ENGINE_INCLUDE_DIR}/engine)

set(ENGINE_HEADERS
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/values.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/positions.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/internal/opening_book.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/evaluation.hpp
    ${ENGINE_INCLUDE_BUILD_DIR}/negamax.hpp
    )

set(ENGINE_SRC
    ${ENGINE_SRC_DIR}/evaluation.cpp
    ${ENGINE_SRC_DIR}/negamax.cpp
    )

add_library(ENGINE ${ENGINE_HEADERS} ${ENGINE_SRC})
target_link_libraries(ENGINE PUBLIC CHESS)
target_include_directories(ENGINE PUBLIC ${ENGINE_INCLUDE_DIR})

if (BUILD_TEST)
    include(${CMAKE_CURRENT_LIST_DIR}/test/CMakeLists.txt)
endif ()

if (BUILD_BIN)
    add_executable(engine ${CMAKE_CURRENT_LIST_DIR}/bin/engine.cpp)
    target_link_libraries(engine PRIVATE ENGINE)
    add_executable(engine_time ${CMAKE_CURRENT_LIST_DIR}/bin/engine_time.cpp)
    target_link_libraries(engine_time PRIVATE ENGINE)
    add_executable(generate_book ${CMAKE_CURRENT_LIST_DIR}/bin/generate_book.cpp ${CMAKE_CURRENT_LIST_DIR}/third_party/indicators/indicators.hpp)
    target_link_libraries(generate_book PRIVATE ENGINE)
    target_include_directories(generate_book PRIVATE ${CMAKE_CURRENT_LIST_DIR}/third_party)
endif ()
