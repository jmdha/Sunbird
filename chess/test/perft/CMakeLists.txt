
## Run From Position
add_executable(RunPosition ${CMAKE_CURRENT_LIST_DIR}/run_from_position.cpp)
target_link_libraries(RunPosition PRIVATE CHESS)

## The positions and numbers are from https://www.chessprogramming.org/Perft_Results
### Initial Pos
add_test(perft_RunPosition_Init_1 RunPosition 20 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 1)
add_test(perft_RunPosition_Init_2 RunPosition 400 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 2)
add_test(perft_RunPosition_Init_3 RunPosition 8902 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 3)
add_test(perft_RunPosition_Init_4 RunPosition 197281 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 4)
add_test(perft_RunPosition_Init_5 RunPosition 4865609 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 5)

### Kiwipete
add_test(perft_RunPosition_Kiwipete_1 RunPosition 48 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " 1)
add_test(perft_RunPosition_Kiwipete_2 RunPosition 2039 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " 2)
add_test(perft_RunPosition_Kiwipete_3 RunPosition 97862 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " 3)
add_test(perft_RunPosition_Kiwipete_4 RunPosition 4085603 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " 4)

### NoCastle
add_test(perft_RunPosition_NoCastle_1 RunPosition 14 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 1)
add_test(perft_RunPosition_NoCastle_2 RunPosition 191 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 2)
add_test(perft_RunPosition_NoCastle_3 RunPosition 2812 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 3)
add_test(perft_RunPosition_NoCastle_4 RunPosition 43238 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 4)
add_test(perft_RunPosition_NoCastle_5 RunPosition 674624 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 5)

### Position 4
add_test(perft_RunPosition_Pos4_1 RunPosition 6 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 1)
add_test(perft_RunPosition_Pos4_2 RunPosition 264 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 2)
add_test(perft_RunPosition_Pos4_3 RunPosition 9467 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 3)
add_test(perft_RunPosition_Pos4_4 RunPosition 422333 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 4)
add_test(perft_RunPosition_Pos4_5 RunPosition 15833292 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 5)

### Position 5
add_test(perft_RunPosition_Pos5_1 RunPosition 44 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  " 1)
add_test(perft_RunPosition_Pos5_2 RunPosition 1486 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  " 2)
add_test(perft_RunPosition_Pos5_3 RunPosition 62379 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  " 3)
add_test(perft_RunPosition_Pos5_4 RunPosition 2103487 "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  " 4)

### Position 6
add_test(perft_RunPosition_Pos6_1 RunPosition 46 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 " 1)
add_test(perft_RunPosition_Pos6_2 RunPosition 2079 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 " 2)
add_test(perft_RunPosition_Pos6_3 RunPosition 89890 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 " 3)
add_test(perft_RunPosition_Pos6_4 RunPosition 3894594 "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 " 4)
