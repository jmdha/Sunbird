############################## DOCTEST SETUP ##############################

set(doctest third_party/doctest.h)

#############################################################################

add_executable(
    TestRunner
    ${CMAKE_CURRENT_LIST_DIR}/test_runner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/board.cpp
    ${CMAKE_CURRENT_LIST_DIR}/masks.cpp
    ${CMAKE_CURRENT_LIST_DIR}/move.cpp
    ${CMAKE_CURRENT_LIST_DIR}/perft.cpp
    ${sources}
)

target_include_directories(
    TestRunner
    PRIVATE
    src
    ${CMAKE_CURRENT_LIST_DIR}/third_party
)
