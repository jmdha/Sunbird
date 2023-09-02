add_executable(Sunbird ${CMAKE_CURRENT_LIST_DIR}/uci.cpp)
target_link_libraries(Sunbird PRIVATE CHESS ENGINE)
