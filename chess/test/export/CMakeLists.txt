# Tests
## Export FEN
## Last parts of the fen isn't implemented for export
add_executable(ExportFEN ${CMAKE_CURRENT_LIST_DIR}/export_FEN.cpp)
target_link_libraries(ExportFEN PRIVATE CHESS)

add_test(Export_FEN_1 ExportFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq ")
add_test(Export_FEN_2 ExportFEN "4r1k1/pp1N1ppp/4qb2/2p5/2P1QP2/5P2/PP1R2PP/2K5 b - ")
add_test(Export_FEN_3 ExportFEN "3qk2r/3nbppp/p1r1p3/8/Q4B2/2N5/PP3PPP/R2R2K1 b k ")
add_test(Export_FEN_4 ExportFEN "8/5ppk/4pb2/1Q6/P5qP/4B3/5P1P/7K b - ")
add_test(Export_FEN_5 ExportFEN "r2r1nk1/np4pp/1q2p1b1/pP1pP3/Pb1P1PP1/4BN2/4B1K1/RN1Q1R2 b - ")

