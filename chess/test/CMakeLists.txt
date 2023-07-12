include(CTest)
enable_testing()

include(${CMAKE_CURRENT_LIST_DIR}/board/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/board_importer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utilities/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/move_gen/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/perft/CMakeLists.txt)

