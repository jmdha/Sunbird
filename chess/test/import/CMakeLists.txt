# Tests
## Import FEN
add_executable(ImportFEN ${CMAKE_CURRENT_LIST_DIR}/import_FEN.cpp)
target_link_libraries(ImportFEN PRIVATE CHESS)

add_test(BoardImporter_ImportFEN_StartPos ImportFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")

