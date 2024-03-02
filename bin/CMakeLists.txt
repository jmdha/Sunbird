add_executable(Sunbird ${CMAKE_CURRENT_LIST_DIR}/uci.cpp ${sources})
target_include_directories(Sunbird PRIVATE src)
add_executable(Benchmark ${CMAKE_CURRENT_LIST_DIR}/benchmark.cpp ${sources})
target_include_directories(Benchmark PRIVATE src)
