
## GetPieceChar
### From char
add_executable(GetPieceCharFromChar ${CMAKE_CURRENT_LIST_DIR}/get_piece_char_from_char.cpp)
target_link_libraries(GetPieceCharFromChar PRIVATE CHESS)

add_test(utilities::getpiecechar::char::P GetPieceCharFromChar 80 P)
add_test(utilities::getpiecechar::char::k GetPieceCharFromChar 107 k)

### From type
add_executable(GetPieceCharFromType ${CMAKE_CURRENT_LIST_DIR}/get_piece_char_from_type.cpp)
target_link_libraries(GetPieceCharFromType PRIVATE CHESS)

add_test(utilities::getpiecechar::type::P GetPieceCharFromType 80 0 0)
add_test(utilities::getpiecechar::type::k GetPieceCharFromType 107 5 1)

## GetPieceType
add_executable(GetPieceType ${CMAKE_CURRENT_LIST_DIR}/get_piece_type.cpp)
target_link_libraries(GetPieceType PRIVATE CHESS)

add_test(utilities::getpiecetype::P GetPieceType 0 P)
add_test(utilities::getpiecetype::p GetPieceType 0 p)
add_test(utilities::getpiecetype::N GetPieceType 1 N)
add_test(utilities::getpiecetype::B GetPieceType 2 B)
add_test(utilities::getpiecetype::R GetPieceType 3 R)
add_test(utilities::getpiecetype::Q GetPieceType 4 Q)
add_test(utilities::getpiecetype::K GetPieceType 5 K)

## GetPieceColor
add_executable(GetPieceColor ${CMAKE_CURRENT_LIST_DIR}/get_piece_color.cpp)
target_link_libraries(GetPieceColor PRIVATE CHESS)

add_test(utilities::getpiececolor::P GetPieceColor 0 P)
add_test(utilities::getpiececolor::p GetPieceColor 1 p)
add_test(utilities::getpiececolor::R GetPieceColor 0 R)
add_test(utilities::getpiececolor::r GetPieceColor 1 r)

## GetSquare
add_executable(GetSquare ${CMAKE_CURRENT_LIST_DIR}/get_square.cpp)
target_link_libraries(GetSquare PRIVATE CHESS)

add_test(utilities::getsquare::A1 GetSquare 0 0 0)
add_test(utilities::getsquare::A2 GetSquare 1 1 0)
add_test(utilities::getsquare::B1 GetSquare 8 0 1)
add_test(utilities::getsquare::H8 GetSquare 63 7 7)
