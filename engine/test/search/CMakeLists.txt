
## Import FEN
add_executable(GetBestMove ${CMAKE_CURRENT_LIST_DIR}/get_best_move.cpp)
target_link_libraries(GetBestMove PRIVATE ENGINE)
add_executable(GetBestMoveSeq ${CMAKE_CURRENT_LIST_DIR}/get_best_move_seq.cpp)
target_link_libraries(GetBestMoveSeq PRIVATE ENGINE)
add_executable(GetAnythingBut ${CMAKE_CURRENT_LIST_DIR}/anything_but.cpp)
target_link_libraries(GetAnythingBut PRIVATE ENGINE)

## Avoid obvious blunder
add_test(search::blunder::1 GetAnythingBut "r2q1rk1/ppp2ppp/2n1bn2/3pB3/2PP4/4PN2/P2N1PPP/R2QKB1R b KQ - 0 9" 10 "d8d6")
add_test(search::blunder::2 GetAnythingBut "r2q1rk1/ppp2ppp/2n1bn2/3pB3/2PP4/4PN2/P2N1PPP/R2QKB1R b KQ - 0 9" 10 "d8d6")
add_test(search::blunder::3 GetAnythingBut "r2q1rk1/ppp2ppp/2n1bn2/3pB3/2PP4/4PN2/P2N1PPP/R2QKB1R b KQ - 0 9" 10 "d8d6")
add_test(search::blunder::4 GetAnythingBut "6k1/5p2/pR3p1p/3p4/3P4/1P3NP1/5PPK/r7 b - - 3 26" 10 "a1h1")
add_test(search::blunder::5 GetAnythingBut "2k2b1r/pp3ppp/3rp3/2p1n3/2B5/P1P1BP1P/1PP1KP2/R6R w - - 4 14" 10 "e3c5")

## Easy to find
add_test(search::freequeen::10 GetBestMove "rnbqkb1r/pppppppp/7n/8/4P1Q1/8/PPPP1PPP/RNB1KBNR b KQkq - 2 2" 10 "h6g4")
add_test(search::freequeen::10w GetBestMove "rnb1kbnr/pppp1ppp/8/4p3/4P1Qq/8/PPPP1PPP/RNB1KBNR w KQkq - 2 3" 10 "g4h4")


## Scenarios
### These are scenarios found during play
### They are tested both as a FEN and as a move sequence
add_test(search::1 GetBestMove "rn1qkb1r/ppp2ppp/4pn2/3p1B2/3P4/4P3/PPP2PPP/RNBQK1NR w KQkq - 0 5" 10 "f5d3")
add_test(search::1s GetBestMoveSeq "d2d4 d7d5 e2e3 g8f6 f1d3 c8f5 d3f5 e7e6" 10 "f5d3")
add_test(search::2 GetBestMove "6k1/5p2/pR3p1p/3p4/3P4/1P3NP1/5PPK/7r w - - 4 27" 10 "h2h1")
add_test(search::3 GetBestMove "rnbqk1nr/ppp2ppp/3bp3/3p4/3P1B2/3Q3P/PPP1PPP1/RN2KBNR b KQkq - 0 4" 10 "d6f4")
add_test(search::3s GetBestMoveSeq "d2d4 d7d5 c1f4 e7e6 d1d3 f8d6 h2h3" 10 "d6f4")
add_test(search::4 GetBestMove "r2qk2r/p1pbbpp1/3p3p/3Q4/3N4/8/PPP2PPP/R1B1R1K1 b kq - 0 16" 10 "e8g8")
add_test(search::5 GetBestMove "8/k7/8/3QQ3/8/PQ2K3/5P2/8 w - - 0 1" 100 "b3b7")
