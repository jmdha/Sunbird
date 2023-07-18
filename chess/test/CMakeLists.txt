include(CTest)
enable_testing()

include(${CMAKE_CURRENT_LIST_DIR}/board/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/bit_shift/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/import/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/export/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utilities/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/move_gen/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/perft/CMakeLists.txt)

