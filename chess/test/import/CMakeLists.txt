# Tests
## Import FEN
add_executable(ImportFEN ${CMAKE_CURRENT_LIST_DIR}/import_FEN.cpp)
target_link_libraries(ImportFEN PRIVATE CHESS)

add_test(Import_FEN_StartPos ImportFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")

## Import Algebraic
add_executable(ImportAlgebraic ${CMAKE_CURRENT_LIST_DIR}/import_algebraic.cpp)
target_link_libraries(ImportAlgebraic PRIVATE CHESS)

add_test(Import_Algebraic_KingsPawn ImportAlgebraic "1. e4" "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq ")
add_test(Import_Algebraic_QueensGambit ImportAlgebraic "1. d4 d5 2. c4" "rnbqkbnr/ppp1pppp/8/3p4/2PP4/8/PP2PPPP/RNBQKBNR b KQkq ")
add_test(Import_Algebraic_FoolsMate ImportAlgebraic "1. e4 e5 2. Bc4 Bc5 3. Qf3 d6 4. Qxf7#" "rnbqk1nr/ppp2Qpp/3p4/2b1p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq ")
add_test(Import_Algebraic_KingCastle ImportAlgebraic "1. g3 d5 2. Bg2 Bf5 3. Nf3 e6 4. O-O" "rn1qkbnr/ppp2ppp/4p3/3p1b2/8/5NP1/PPPPPPBP/RNBQ1RK1 b kq ")
add_test(Import_Algebraic_QueenCastle ImportAlgebraic "1. d4 d5 2. Bf4 Bf5 3. Nc3 e6 4. e3 Nf6 5. Qd2 Bb4 6. O-O-O" "rn1qk2r/ppp2ppp/4pn2/3p1b2/1b1P1B2/2N1P3/PPPQ1PPP/2KR1BNR b kq ")
add_test(Import_Algebraic_EnPassant ImportAlgebraic "1. d4 e5 2. d5 c5 3. dxc6" "rnbqkbnr/pp1p1ppp/2P5/4p3/8/8/PPP1PPPP/RNBQKBNR b KQkq ")
add_test(Import_Algebraic_DoubleEnPassant_a ImportAlgebraic "1. d4 Na6 2. d5 Nb8 3. f4 Na6 4. f5 e5 5. dxe6" "r1bqkbnr/pppp1ppp/n3P3/5P2/8/8/PPP1P1PP/RNBQKBNR b KQkq ")
add_test(Import_Algebraic_DoubleEnPassant_b ImportAlgebraic "1. d4 Na6 2. d5 Nb8 3. f4 Na6 4. f5 e5 5. fxe6" "r1bqkbnr/pppp1ppp/n3P3/3P4/8/8/PPP1P1PP/RNBQKBNR b KQkq ")
add_test(Import_Algebraic_Promotion_Q ImportAlgebraic "1. h4 h5 2. g4 hxg4 3. h5 Rh6 4. Nf3 Ra6 5. h6 Rb6 6. h7 Ra6 7. h8=Q" "rnbqkbnQ/ppppppp1/r7/8/6p1/5N2/PPPPPP2/RNBQKB1R b KQq ")
add_test(Import_Algebraic_Promotion_N ImportAlgebraic "1. h4 h5 2. g4 hxg4 3. h5 Rh6 4. Nf3 Ra6 5. h6 Rb6 6. h7 Ra6 7. h8=N" "rnbqkbnN/ppppppp1/r7/8/6p1/5N2/PPPPPP2/RNBQKB1R b KQq ")
add_test(Import_Algebraic_Promotion_R ImportAlgebraic "1. h4 h5 2. g4 hxg4 3. h5 Rh6 4. Nf3 Ra6 5. h6 Rb6 6. h7 Ra6 7. h8=R" "rnbqkbnR/ppppppp1/r7/8/6p1/5N2/PPPPPP2/RNBQKB1R b KQq ")
add_test(Import_Algebraic_Promotion_B ImportAlgebraic "1. h4 h5 2. g4 hxg4 3. h5 Rh6 4. Nf3 Ra6 5. h6 Rb6 6. h7 Ra6 7. h8=B" "rnbqkbnB/ppppppp1/r7/8/6p1/5N2/PPPPPP2/RNBQKB1R b KQq ")
add_test(Import_Algebraic_Promotion_q ImportAlgebraic "1. a4 b5 2. axb5 a5 3. Ra3 a4 4. Rb3 a3 5. Rc3 a2 6. Rb3 a1=Q" "rnbqkbnr/2pppppp/8/1P6/8/1R6/1PPPPPPP/qNBQKBNR w Kkq ")
add_test(Import_Algebraic_Promotion_n ImportAlgebraic "1. a4 b5 2. axb5 a5 3. Ra3 a4 4. Rb3 a3 5. Rc3 a2 6. Rb3 a1=N" "rnbqkbnr/2pppppp/8/1P6/8/1R6/1PPPPPPP/nNBQKBNR w Kkq ")
add_test(Import_Algebraic_Promotion_r ImportAlgebraic "1. a4 b5 2. axb5 a5 3. Ra3 a4 4. Rb3 a3 5. Rc3 a2 6. Rb3 a1=R" "rnbqkbnr/2pppppp/8/1P6/8/1R6/1PPPPPPP/rNBQKBNR w Kkq ")
add_test(Import_Algebraic_Promotion_b ImportAlgebraic "1. a4 b5 2. axb5 a5 3. Ra3 a4 4. Rb3 a3 5. Rc3 a2 6. Rb3 a1=B" "rnbqkbnr/2pppppp/8/1P6/8/1R6/1PPPPPPP/bNBQKBNR w Kkq ")
add_test(Import_Algebraic_CapturePromotion_Q ImportAlgebraic "1. a4 Nh6 2. a5 Ng8 3. a6 Nh6 4. axb7 Ng8 5. bxa8=Q" "Qnbqkbnr/p1pppppp/8/8/8/8/1PPPPPPP/RNBQKBNR b KQk ")
add_test(Import_Algebraic_CapturePromotion_N ImportAlgebraic "1. a4 Nh6 2. a5 Ng8 3. a6 Nh6 4. axb7 Ng8 5. bxa8=N" "Nnbqkbnr/p1pppppp/8/8/8/8/1PPPPPPP/RNBQKBNR b KQk ")
add_test(Import_Algebraic_CapturePromotion_R ImportAlgebraic "1. a4 Nh6 2. a5 Ng8 3. a6 Nh6 4. axb7 Ng8 5. bxa8=R" "Rnbqkbnr/p1pppppp/8/8/8/8/1PPPPPPP/RNBQKBNR b KQk ")
add_test(Import_Algebraic_CapturePromotion_B ImportAlgebraic "1. a4 Nh6 2. a5 Ng8 3. a6 Nh6 4. axb7 Ng8 5. bxa8=B" "Bnbqkbnr/p1pppppp/8/8/8/8/1PPPPPPP/RNBQKBNR b KQk ")
add_test(Import_Algebraic_CapturePromotion_q ImportAlgebraic "1. Nc3 h5 2. Nb1 h4 3. Nc3 h3 4. Nb1 hxg2 5. Nc3 gxh1=Q" "rnbqkbnr/ppppppp1/8/8/8/2N5/PPPPPP1P/R1BQKBNq w Qkq ")
add_test(Import_Algebraic_CapturePromotion_n ImportAlgebraic "1. Nc3 h5 2. Nb1 h4 3. Nc3 h3 4. Nb1 hxg2 5. Nc3 gxh1=N" "rnbqkbnr/ppppppp1/8/8/8/2N5/PPPPPP1P/R1BQKBNn w Qkq ")
add_test(Import_Algebraic_CapturePromotion_r ImportAlgebraic "1. Nc3 h5 2. Nb1 h4 3. Nc3 h3 4. Nb1 hxg2 5. Nc3 gxh1=R" "rnbqkbnr/ppppppp1/8/8/8/2N5/PPPPPP1P/R1BQKBNr w Qkq ")
add_test(Import_Algebraic_CapturePromotion_b ImportAlgebraic "1. Nc3 h5 2. Nb1 h4 3. Nc3 h3 4. Nb1 hxg2 5. Nc3 gxh1=B" "rnbqkbnr/ppppppp1/8/8/8/2N5/PPPPPP1P/R1BQKBNb w Qkq ")

