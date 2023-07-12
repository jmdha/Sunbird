# Tests
## Import FEN
add_executable(GetBestMove ${CMAKE_CURRENT_LIST_DIR}/get_best_move.cpp)
target_link_libraries(GetBestMove PRIVATE ENGINE)
add_executable(GetBestMoveSeq ${CMAKE_CURRENT_LIST_DIR}/get_best_move_seq.cpp)
target_link_libraries(GetBestMoveSeq PRIVATE ENGINE)
add_executable(GetAnythingBut ${CMAKE_CURRENT_LIST_DIR}/anything_but.cpp)
target_link_libraries(GetAnythingBut PRIVATE ENGINE)

## Easy to find
add_test(GetBestMove_FreeQueen_1 GetBestMove "rnbqkb1r/pppppppp/7n/8/4P1Q1/8/PPPP1PPP/RNB1KBNR b KQkq - 2 2" 1 "h6g4")
add_test(GetBestMove_FreeQueen_2 GetBestMove "rnbqkb1r/pppppppp/7n/8/4P1Q1/8/PPPP1PPP/RNB1KBNR b KQkq - 2 2" 2 "h6g4")
add_test(GetBestMove_FreeQueen_3 GetBestMove "rnbqkb1r/pppppppp/7n/8/4P1Q1/8/PPPP1PPP/RNB1KBNR b KQkq - 2 2" 3 "h6g4")
add_test(GetBestMove_FreeQueen_1w GetBestMove "rnb1kbnr/pppp1ppp/8/4p3/4P1Qq/8/PPPP1PPP/RNB1KBNR w KQkq - 2 3" 1 "g4h4")
add_test(GetBestMove_FreeQueen_2w GetBestMove "rnb1kbnr/pppp1ppp/8/4p3/4P1Qq/8/PPPP1PPP/RNB1KBNR w KQkq - 2 3" 2 "g4h4")
add_test(GetBestMove_FreeQueen_3w GetBestMove "rnb1kbnr/pppp1ppp/8/4p3/4P1Qq/8/PPPP1PPP/RNB1KBNR w KQkq - 2 3" 3 "g4h4")

## Avoid obvious blunder
add_test(AnythingBut_MoveBlunder_1 GetAnythingBut "r2q1rk1/ppp2ppp/2n1bn2/3pB3/2PP4/4PN2/P2N1PPP/R2QKB1R b KQ - 0 9" 1 "d8d6")
add_test(AnythingBut_MoveBlunder_2 GetAnythingBut "r2q1rk1/ppp2ppp/2n1bn2/3pB3/2PP4/4PN2/P2N1PPP/R2QKB1R b KQ - 0 9" 2 "d8d6")
add_test(AnythingBut_MoveBlunder_3 GetAnythingBut "r2q1rk1/ppp2ppp/2n1bn2/3pB3/2PP4/4PN2/P2N1PPP/R2QKB1R b KQ - 0 9" 3 "d8d6")
add_test(AnythingBut_MoveBlunder_4 GetAnythingBut "6k1/5p2/pR3p1p/3p4/3P4/1P3NP1/5PPK/r7 b - - 3 26" 3 "a1h1")

## Scenarios
### These are scenarios found during play
### They are tested both as a FEN and as a move sequence
add_test(GetBestMove_1 GetBestMove "rn1qkb1r/ppp2ppp/4pn2/3p1B2/3P4/4P3/PPP2PPP/RNBQK1NR w KQkq - 0 5" 2 "f5d3")
add_test(GetBestMove_1s GetBestMoveSeq "d2d4 d7d5 e2e3 g8f6 f1d3 c8f5 d3f5 e7e6" 2 "f5d3")
add_test(GetBestMove_2 GetBestMove "6k1/5p2/pR3p1p/3p4/3P4/1P3NP1/5PPK/7r w - - 4 27" 3 "h2h1")
add_test(GetBestMove_3 GetBestMove "rnbqk1nr/ppp2ppp/3bp3/3p4/3P1B2/3Q3P/PPP1PPP1/RN2KBNR b KQkq - 0 4" 3 "d6f4")
add_test(GetBestMove_3s GetBestMoveSeq "d2d4 d7d5 c1f4 e7e6 d1d3 f8d6 h2h3" 3 "d6f4")

