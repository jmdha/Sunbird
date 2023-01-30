# Tests
## Import FEN
add_executable(ImportFEN tests/board_importer/import_FEN.cpp "classes/board.cpp" "classes/board_importer.cpp")

add_test(BoardImporter_ImportFEN_StartPos ImportFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")