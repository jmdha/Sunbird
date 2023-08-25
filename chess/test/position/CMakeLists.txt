# Tests
## Import FEN
add_executable(SetEP ${CMAKE_CURRENT_LIST_DIR}/set_ep.cpp)
target_link_libraries(SetEP PRIVATE CHESS)

add_test(Board_Position_SetEP_A SetEP "a")
add_test(Board_Position_SetEP_B SetEP "b")
add_test(Board_Position_SetEP_C SetEP "c")
add_test(Board_Position_SetEP_D SetEP "d")
add_test(Board_Position_SetEP_E SetEP "e")
add_test(Board_Position_SetEP_F SetEP "f")
add_test(Board_Position_SetEP_G SetEP "g")
add_test(Board_Position_SetEP_H SetEP "h")
add_test(Board_Position_SetEP_None SetEP "none")

