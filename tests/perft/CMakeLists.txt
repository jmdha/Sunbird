# Tests
## Run From Position
add_executable(RunPosition tests/perft/run_from_position.cpp ${sources} "classes/perft.cpp")

### Initial Pos
add_test(Perft_RunPosition_Init_0 RunPosition 1 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 0)
add_test(Perft_RunPosition_Init_1 RunPosition 20 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 1)
add_test(Perft_RunPosition_Init_2 RunPosition 400 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 2)
add_test(Perft_RunPosition_Init_3 RunPosition 8902 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 3)
add_test(Perft_RunPosition_Init_4 RunPosition 197281 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 4)
add_test(Perft_RunPosition_Init_5 RunPosition 4865609 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 5)

### Kiwipete
add_test(Perft_RunPosition_Kiwipete_1 RunPosition 48 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " 1)
add_test(Perft_RunPosition_Kiwipete_2 RunPosition 2039 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " 2)
add_test(Perft_RunPosition_Kiwipete_3 RunPosition 97862 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " 3)
add_test(Perft_RunPosition_Kiwipete_4 RunPosition 4085603 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - " 4)

### NoCastle
add_test(Perft_RunPosition_NoCastle_1 RunPosition 14 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 1)
add_test(Perft_RunPosition_NoCastle_2 RunPosition 191 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 2)
add_test(Perft_RunPosition_NoCastle_3 RunPosition 2812 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 3)
add_test(Perft_RunPosition_NoCastle_4 RunPosition 43238 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 4)
add_test(Perft_RunPosition_NoCastle_5 RunPosition 674624 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - " 5)