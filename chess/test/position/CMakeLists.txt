
## Import FEN
add_executable(SetEP ${CMAKE_CURRENT_LIST_DIR}/set_ep.cpp)
target_link_libraries(SetEP PRIVATE CHESS)

add_test(board::position::setEP::A SetEP "a")
add_test(board::position::setEP::B SetEP "b")
add_test(board::position::setEP::C SetEP "c")
add_test(board::position::setEP::D SetEP "d")
add_test(board::position::setEP::E SetEP "e")
add_test(board::position::setEP::F SetEP "f")
add_test(board::position::setEP::G SetEP "g")
add_test(board::position::setEP::H SetEP "h")
add_test(board::position::setEP::None SetEP "none")
