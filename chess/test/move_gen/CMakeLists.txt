
## King Safe
add_executable(IsKingSafe ${CMAKE_CURRENT_LIST_DIR}/king_safe.cpp)
target_link_libraries(IsKingSafe PRIVATE CHESS)

add_test(moveGen::iskingsafe::startpos IsKingSafe "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 1)
add_test(moveGen::iskingsafe::purerook IsKingSafe "bbbbbbbb/bbbbbbbb/bbbbbbbb/1bbbbbbb/b1bbbbb1/bb1bbb1b/bbb1b1bb/bbbbKbbb w - - 0 1" 1)
add_test(moveGen::iskingsafe::mixed IsKingSafe "4R3/2B5/1R6/3k4/5R2/4B3/2R5/8 b - - 0 1" 1)
add_test(moveGen::iskingsafe::rook IsKingSafe "3r4/8/8/8/8/8/8/3K4 w - - 0 1" 0)
add_test(moveGen::iskingsafe::bishop IsKingSafe "8/8/8/3k4/8/8/8/7B b - - 0 1" 0)
add_test(moveGen::iskingsafe::knights IsKingSafe "8/8/8/8/8/1n1n1n2/2nnn3/1nnKnn2 w - - 0 1" 1)
add_test(moveGen::iskingsafe::knight IsKingSafe "8/8/8/8/8/4n3/8/3K4 w - - 0 1" 0)
add_test(moveGen::iskingsafe::pawn::1::w IsKingSafe "8/8/1ppppp2/1p1p1p2/1ppKpp2/1ppppp2/8/8 w - - 0 1" 1)
add_test(moveGen::iskingsafe::pawn::2::w IsKingSafe "8/8/8/2p5/3K4/8/8/8 w - - 0 1" 0)
add_test(moveGen::iskingsafe::pawn::3::w IsKingSafe "8/8/8/4p3/3K4/8/8/8 w - - 0 1" 0)
add_test(moveGen::iskingsafe::pawn::1::b IsKingSafe "8/8/1PPPPP2/1PPPPP2/1PPkPP2/1P1P1P2/1PPPPP2/8 b - - 0 1" 1)
add_test(moveGen::iskingsafe::pawn::2::b IsKingSafe "8/8/8/8/3k4/2P5/8/8 b - - 0 1" 0)
add_test(moveGen::iskingsafe::pawn::3::B IsKingSafe "8/8/8/8/3k4/4P3/8/8 b - - 0 1" 0)

add_test(moveGen::iskingsafe::blocked::rookfriendly IsKingSafe "4r3/8/8/8/8/8/4R3/4K3 w - - 0 1" 1)
add_test(moveGen::iskingsafe::blocked::rookenemy IsKingSafe "4r3/8/8/8/8/8/4b3/4K3 w - - 0 1" 1)
add_test(moveGen::iskingsafe::blocked::bishopsfriendly IsKingSafe "8/8/8/8/1b6/8/3B4/4K3 w - - 0 1" 1)
add_test(moveGen::iskingsafe::blocked::bishopsenemy IsKingSafe "8/8/8/8/1b6/8/3r4/4K3 w - - 0 1" 1)

### These are positions which have been incorrect before
add_test(movegen::iskingsafe::multi IsKingSafe "r3k2r/p4ppp/1pnbB3/4p3/PpP5/3P1Pn1/8/3Kq3 w kq - 0 23" 0)

## PawnMoves
add_executable(GeneratePawnMoves ${CMAKE_CURRENT_LIST_DIR}/pawn_move.cpp)
target_link_libraries(GeneratePawnMoves PRIVATE CHESS)

add_test(movegen::pawn::moveinitial GeneratePawnMoves            16  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 1)
add_test(movegen::pawn::move::onlypawns::w GeneratePawnMoves        16  "8/pppppppp/8/8/8/8/PPPPPPPP/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::move::onlypawns::b GeneratePawnMoves        16  "8/pppppppp/8/8/8/8/PPPPPPPP/8 b KQkq - 0 1" 1)

add_test(movegen::pawn::move::single::w GeneratePawnMoves           8  "8/8/pppppppp/8/8/PPPPPPPP/8/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::move::single::b GeneratePawnMoves           8  "8/8/pppppppp/8/8/PPPPPPPP/8/8 b KQkq - 0 1" 1)

add_test(movegen::pawn::move::blocker::w GeneratePawnMoves          0   "8/8/8/8/8/Q7/P7/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockeroffset1::w GeneratePawnMoves   1   "8/8/8/8/Q7/8/P7/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockeroffset2::w GeneratePawnMoves   2   "8/8/8/Q7/8/8/P7/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::move::blocker::b GeneratePawnMoves          0   "8/p7/q7/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockeroffset1::b GeneratePawnMoves   1   "8/p7/8/q7/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockeroffset2::b GeneratePawnMoves   2   "8/p7/8/8/q7/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::pawn::move::blockermulti::w GeneratePawnMoves          0   "8/8/8/8/8/QQQQQQQQ/PPPPPPPP/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockermultioffset1::w GeneratePawnMoves   8   "8/8/8/8/QQQQQQQQ/8/PPPPPPPP/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockermultioffset2::w GeneratePawnMoves   16   "8/8/8/QQQQQQQQ/8/8/PPPPPPPP/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockermulti::b GeneratePawnMoves          0   "8/pppppppp/qqqqqqqq/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockermultioffset1::b GeneratePawnMoves   8   "8/pppppppp/8/qqqqqqqq/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::pawn::move::blockermultioffset2::b GeneratePawnMoves   16   "8/pppppppp/8/8/qqqqqqqq/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::pawn::capture::left::w GeneratePawnMoves      3   "8/8/8/8/8/p7/1P6/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::capture::right::w GeneratePawnMoves     3   "8/8/8/8/8/2p5/1P6/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::capture::both::w GeneratePawnMoves      4   "8/8/8/8/8/p1p5/1P6/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::capture::left::b GeneratePawnMoves      3   "8/p7/1P6/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::pawn::capture::right::b GeneratePawnMoves     3   "8/2p5/1P6/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::pawn::capture::both::b GeneratePawnMoves      4   "8/1p6/p1p5/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::pawn::capture::leftmultiple::w GeneratePawnMoves      9   "8/8/8/8/8/p2p2p1/1P2P2P/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::capture::rightmultiple::w GeneratePawnMoves     9   "8/8/8/8/8/1p2p2p/P2P2P1/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::capture::bothmultiple::w GeneratePawnMoves      14  "8/8/8/8/8/pppppppp/PPPPPPPP/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::capture::leftmultiple::b GeneratePawnMoves      9   "8/p2p2p1/1P2P2P/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::pawn::capture::rightmultiple::b GeneratePawnMoves     9   "8/1p2p2p/P2P2P1/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::pawn::capture::bothmultiple::b GeneratePawnMoves      14  "8/pppppppp/PPPPPPPP/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::pawn::capture::noedge::w GeneratePawnMoves      2   "8/8/8/8/8/8/P6p/8 w KQkq - 0 1" 1)
add_test(movegen::pawn::capture::noedge::b GeneratePawnMoves      2   "8/p6P/8/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::pawn::EP GeneratePawnMoves      2   "7k/8/8/1Pp4r/K7/8/8/8 w - c6 0 2")
add_test(movegen::pawn::EP::KingSafety GeneratePawnMoves      1   "7k/8/8/KPp4r/8/8/8/8 w - c6 0 2")

add_test(movegen::pawn::kingsafety::singlemove GeneratePawnMoves   1   "8/8/8/8/4P3/r6K/6P1/8 w - - 0 1" 0)
add_test(movegen::pawn::kingsafety::doublemove GeneratePawnMoves   5   "8/2p1p1p1/3p1p2/1k5R/8/8/8/8 b - - 0 1" 0)
add_test(movegen::pawn::kingsafety::capture GeneratePawnMoves   2   "8/8/6p1/1k5R/8/3p4/4R3/8 b - - 0 1" 0)

add_test(movegen::pawn::promotion::w GeneratePawnMoves   8   "4k3/P6P/8/8/8/8/8/4K3 w - - 0 1" 1)
add_test(movegen::pawn::promotion::b GeneratePawnMoves   8   "4k3/8/8/8/8/8/p6p/4K3 b - - 0 1" 1)
add_test(movegen::pawn::promotion::capture::w GeneratePawnMoves   24   "r1q1kr1n/1P4P1/8/8/8/8/8/4K3 w - - 0 1" 1)
add_test(movegen::pawn::promotion::capture::b GeneratePawnMoves   24   "4k3/8/8/8/8/8/1p4p1/N1Q1KB1R b - - 0 1" 1)

## RookMoves
add_executable(GenerateRookMoves ${CMAKE_CURRENT_LIST_DIR}/rook_move.cpp)
target_link_libraries(GenerateRookMoves PRIVATE CHESS)

add_test(movegen::rook::move::corner::1::w GenerateRookMoves    14  "R7/8/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::corner::2::w GenerateRookMoves    14  "7R/8/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::corner::3::w GenerateRookMoves    14  "8/8/8/8/8/8/8/R7 w KQkq - 0 1" 1)
add_test(movegen::rook::move::corner::4::w GenerateRookMoves    14  "8/8/8/8/8/8/8/7R w KQkq - 0 1" 1)

add_test(movegen::rook::move::corner::1::b GenerateRookMoves    14  "r7/8/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::rook::move::corner::2::b GenerateRookMoves    14  "7r/8/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::rook::move::corner::3::b GenerateRookMoves    14  "8/8/8/8/8/8/8/r7 b KQkq - 0 1" 1)
add_test(movegen::rook::move::corner::4::b GenerateRookMoves    14  "8/8/8/8/8/8/8/7r b KQkq - 0 1" 1)

add_test(movegen::rook::move::middle::1::w GenerateRookMoves    14  "8/8/8/3R4/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::middle::2::w GenerateRookMoves    14  "8/8/8/4R3/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::middle::3::w GenerateRookMoves    14  "8/8/8/8/3R4/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::middle::4::w GenerateRookMoves    14  "8/8/8/8/3R4/8/8/8 w KQkq - 0 1" 1)

add_test(movegen::rook::move::middle::1::b GenerateRookMoves    14  "8/8/8/3r4/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::rook::move::middle::2::b GenerateRookMoves    14  "8/8/8/4r3/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::rook::move::middle::3::b GenerateRookMoves    14  "8/8/8/8/3r4/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::rook::move::middle::4::b GenerateRookMoves    14  "8/8/8/8/4r3/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::rook::move::blocker::w GenerateRookMoves              0  "RB6/B7/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::blockeroffset::w GenerateRookMoves       2  "R1B5/8/B7/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::blocker::b GenerateRookMoves              0  "rb6/b7/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::rook::move::blockeroffset::b GenerateRookMoves       2  "r1b5/8/b7/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::rook::move::attack::blocker::w GenerateRookMoves              2  "Rb6/b7/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::attack::blockeroffset::w GenerateRookMoves       4  "R1b5/8/b7/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::attack::blocker::b GenerateRookMoves              2  "rB6/B7/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::rook::move::attack::blockeroffset::b GenerateRookMoves       4  "r1B5/8/B7/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::rook::move::attack::multiple::blockeroffset::w GenerateRookMoves       24  "8/1R1bb1R1/8/1b4b1/1b4b1/8/1R1bb1R1/8 w KQkq - 0 1" 1)
add_test(movegen::rook::move::attack::multiple::blockeroffset::b GenerateRookMoves       24  "8/1r1BB1r1/8/1B4B1/1B4B1/8/1r1BB1r1/8 b KQkq - 0 1" 1)

add_test(movegen::rook::kingsafety::move GenerateRookMoves   1   "8/2r5/8/8/8/6R1/2K5/8 w - - 0 1" 0)
add_test(movegen::rook::kingsafety::capture GenerateRookMoves   1   "8/3k4/8/8/8/8/3R2r1/8 b - - 0 1" 0)
add_test(movegen::rook::kingsafety::none GenerateRookMoves   0   "8/3k4/8/8/3R4/8/3R2r1/8 b - - 0 1" 0)

## BishopMoves
add_executable(GenerateBishopMoves ${CMAKE_CURRENT_LIST_DIR}/bishop_move.cpp)
target_link_libraries(GenerateBishopMoves PRIVATE CHESS)

add_test(movegen::bishop::move::corner::1::w GenerateBishopMoves    7  "B7/8/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::move::corner::2::w GenerateBishopMoves    7  "7B/8/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::move::corner::3::w GenerateBishopMoves    7  "8/8/8/8/8/8/8/B7 w KQkq - 0 1" 1)
add_test(movegen::bishop::move::corner::4::w GenerateBishopMoves    7  "8/8/8/8/8/8/8/7B w KQkq - 0 1" 1)

add_test(movegen::bishop::move::corner::1::b GenerateBishopMoves    7  "b7/8/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::bishop::move::corner::2::b GenerateBishopMoves    7  "7b/8/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::bishop::move::corner::3::b GenerateBishopMoves    7  "8/8/8/8/8/8/8/b7 b KQkq - 0 1" 1)
add_test(movegen::bishop::move::corner::4::b GenerateBishopMoves    7  "8/8/8/8/8/8/8/7b b KQkq - 0 1" 1)

add_test(movegen::bishop::movemiddle::1::w GenerateBishopMoves    13  "8/8/8/3B4/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::movemiddle::2::w GenerateBishopMoves    13  "8/8/8/4B3/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::movemiddle::3::w GenerateBishopMoves    13  "8/8/8/8/3B4/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::movemiddle::4::w GenerateBishopMoves    13  "8/8/8/8/3B4/8/8/8 w KQkq - 0 1" 1)

add_test(movegen::bishop::movemiddle::1::b GenerateBishopMoves    13  "8/8/8/3b4/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::bishop::movemiddle::2::b GenerateBishopMoves    13  "8/8/8/4b3/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::bishop::movemiddle::3::b GenerateBishopMoves    13  "8/8/8/8/3b4/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::bishop::movemiddle::4::b GenerateBishopMoves    13  "8/8/8/8/4b3/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::bishop::moveblocker::w GenerateBishopMoves              0  "B7/1R6/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::moveblockeroffset::w GenerateBishopMoves       1  "B7/8/2R5/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::moveblocker::b GenerateBishopMoves              0  "b7/1r6/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::bishop::moveblockeroffset::b GenerateBishopMoves       1  "b7/8/2r5/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::bishop::moveattack::blocker::w           GenerateBishopMoves     1 "B7/1r6/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::moveattack::blockeroffset::w    GenerateBishopMoves     2 "B7/8/2r5/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::bishop::moveattack::blocker::b           GenerateBishopMoves     1 "b7/1R6/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::bishop::moveattack::blockeroffset1::b    GenerateBishopMoves     2 "b7/8/2R5/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::bishop::moveattack::blockeroffset::multiple::w GenerateBishopMoves       24  "r6r/3rr3/2B2B2/1r4r1/1r4r1/2B2B2/3rr3/r6r w - - 0 1" 1)
add_test(movegen::bishop::moveattack::blockeroffset::multiple::b GenerateBishopMoves       24  "R6R/3RR2R/2b2b2/1R4R1/1R4R1/2b2b2/3RR3/R6R b - - 0 1" 1)

add_test(movegen::bishop::kingsafety::move GenerateBishopMoves   1   "8/5B2/8/1K6/8/8/8/5b2 w - - 0 1" 0)
add_test(movegen::bishop::kingsafety::capture GenerateBishopMoves   1   "8/2k5/8/8/5B2/8/3b4/8 b - - 0 1" 0)
add_test(movegen::bishop::kingsafety::none GenerateBishopMoves   0   "8/2k5/8/4B3/5B2/8/3b4/8 b - - 0 1" 0)

## QueenMoves
add_executable(GenerateQueenMoves ${CMAKE_CURRENT_LIST_DIR}/queen_move.cpp)
target_link_libraries(GenerateQueenMoves PRIVATE CHESS)

add_test(movegen::queen::move::corner::1::w GenerateQueenMoves    21  "Q7/8/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::corner::2::w GenerateQueenMoves    21  "7Q/8/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::corner::3::w GenerateQueenMoves    21  "8/8/8/8/8/8/8/Q7 w KQkq - 0 1" 1)
add_test(movegen::queen::move::corner::4::w GenerateQueenMoves    21  "8/8/8/8/8/8/8/7Q w KQkq - 0 1" 1)

add_test(movegen::queen::move::corner::1::b GenerateQueenMoves    21  "q7/8/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::queen::move::corner::2::b GenerateQueenMoves    21  "7q/8/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::queen::move::corner::3::b GenerateQueenMoves    21  "8/8/8/8/8/8/8/q7 b KQkq - 0 1" 1)
add_test(movegen::queen::move::corner::4::b GenerateQueenMoves    21  "8/8/8/8/8/8/8/7q b KQkq - 0 1" 1)

add_test(movegen::queen::move::middle::1::w GenerateQueenMoves    27  "8/8/8/3Q4/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::middle::2::w GenerateQueenMoves    27  "8/8/8/4Q3/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::middle::3::w GenerateQueenMoves    27  "8/8/8/8/3Q4/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::middle::4::w GenerateQueenMoves    27  "8/8/8/8/3Q4/8/8/8 w KQkq - 0 1" 1)

add_test(movegen::queen::move::middle::1::b GenerateQueenMoves    27  "8/8/8/3q4/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::queen::move::middle::2::b GenerateQueenMoves    27  "8/8/8/4q3/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::queen::move::middle::3::b GenerateQueenMoves    27  "8/8/8/8/3q4/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::queen::move::middle::4::b GenerateQueenMoves    27  "8/8/8/8/4q3/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::queen::move::blocker::w           GenerateQueenMoves  0  "QR6/RR6/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::blockerOffset1::w    GenerateQueenMoves  3  "Q1R5/8/R1R5/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::blocker::b           GenerateQueenMoves  0  "qr6/rr6/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::queen::move::blockerOffset1::b    GenerateQueenMoves  3  "q1r5/8/r1r5/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::queen::move::attackblocker::w           GenerateQueenMoves     3 "Qr6/rr6/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::attackblockeroffset::w    GenerateQueenMoves     6 "Q1r5/8/r1r5/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::queen::move::attackblocker::b           GenerateQueenMoves     3 "qR6/RR6/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::queen::move::attackblockeroffset::b    GenerateQueenMoves     6 "q1R5/8/R1R5/8/8/8/8/8 b KQkq - 0 1" 1)

add_test(movegen::queen::move::attackmultiple::blockeroffset::w GenerateQueenMoves       60  "2r2r2/1r4r1/r1Q2Q1r/8/8/r1Q2Q1r/1r4r1/2r2r2 w - - 0 1" 1)
add_test(movegen::queen::move::attackmultiple::blockeroffset::b GenerateQueenMoves       60  "2R2R2/1R4R1/R1q2q1R/8/8/R1q2q1R/1R4R1/2R2R2 b - - 0 1" 1)

add_test(movegen::queen::KingSafety::move GenerateQueenMoves   2   "8/5Q2/2K5/8/8/8/6q1/8 w - - 0 1" 0)
add_test(movegen::queen::KingSafety::capture GenerateQueenMoves   1   "7Q/8/8/8/8/8/8/k6q b - - 0 1" 0)
add_test(movegen::queen::KingSafety::none GenerateQueenMoves   0   "7Q/8/5Q2/8/8/8/8/k6q b - - 0 1" 0)

## KnightMoves
add_executable(GenerateKnightMoves ${CMAKE_CURRENT_LIST_DIR}/knight_move.cpp)
target_link_libraries(GenerateKnightMoves PRIVATE CHESS)

add_test(movegen::knight::move::corner::1::w GenerateKnightMoves    2  "N7/8/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::knight::move::corner::2::w GenerateKnightMoves    2  "7N/8/8/8/8/8/8/8 w KQkq - 0 1" 1)
add_test(movegen::knight::move::corner::3::w GenerateKnightMoves    2  "8/8/8/8/8/8/8/N7 w KQkq - 0 1" 1)
add_test(movegen::knight::move::corner::4::w GenerateKnightMoves    2  "8/8/8/8/8/8/8/7N w KQkq - 0 1" 1)

add_test(movegen::knight::move::corner::1::b GenerateKnightMoves    2  "n7/8/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::knight::move::corner::2::b GenerateKnightMoves    2  "7n/8/8/8/8/8/8/8 b KQkq - 0 1" 1)
add_test(movegen::knight::move::corner::3::b GenerateKnightMoves    2  "8/8/8/8/8/8/8/n7 b KQkq - 0 1" 1)
add_test(movegen::knight::move::corner::4::b GenerateKnightMoves    2  "8/8/8/8/8/8/8/7n b KQkq - 0 1" 1)

add_test(movegen::knight::moveblocked::w GenerateKnightMoves    24  "8/2R2R2/1R4R1/3NN3/3NN3/1R4R1/2R2R2/8 w - - 0 1" 1)
add_test(movegen::knight::moveblocked::b GenerateKnightMoves    24  "8/2r2r2/1r4r1/3nn3/3nn3/1r4r1/2r2r2/8 b - - 0 1" 1)

add_test(movegen::knight::moveattack::w GenerateKnightMoves    32  "8/2r2r2/1r4r1/3NN3/3NN3/1r4r1/2r2r2/8 w - - 0 1" 1)
add_test(movegen::knight::moveattack::b GenerateKnightMoves    32  "8/2R2R2/1R4R1/3nn3/3nn3/1R4R1/2R2R2/8 b - - 0 1" 1)

add_test(movegen::knight::kingsafety::move GenerateKnightMoves   2   "8/8/3K4/8/8/b2N4/8/8 w - - 0 1" 0)
add_test(movegen::knight::kingsafety::capture GenerateKnightMoves   1   "8/4N3/8/3n1k2/8/8/8/8 b - - 0 1" 0)

## KingMoves
add_executable(GenerateKingMoves ${CMAKE_CURRENT_LIST_DIR}/king_move.cpp)
target_link_libraries(GenerateKingMoves PRIVATE CHESS)

add_test(movegen::king::move::corner1::w GenerateKingMoves    3  "K7/8/8/8/8/8/8/8 w - - " 1)
add_test(movegen::king::move::corner2::w GenerateKingMoves    3  "7K/8/8/8/8/8/8/8 w - - " 1)
add_test(movegen::king::move::corner3::w GenerateKingMoves    3  "8/8/8/8/8/8/8/K7 w - - " 1)
add_test(movegen::king::move::corner4::w GenerateKingMoves    3  "8/8/8/8/8/8/8/7K w - - " 1)

add_test(movegen::king::move::corner1::b GenerateKingMoves    3  "k7/8/8/8/8/8/8/8 b - - " 1)
add_test(movegen::king::move::corner2::b GenerateKingMoves    3  "7k/8/8/8/8/8/8/8 b - - " 1)
add_test(movegen::king::move::corner3::b GenerateKingMoves    3  "8/8/8/8/8/8/8/k7 b - - " 1)
add_test(movegen::king::move::corner4::b GenerateKingMoves    3  "8/8/8/8/8/8/8/7k b - - " 1)

add_test(movegen::king::move::blocked::w GenerateKingMoves    0  "8/8/8/2RRR3/2RKR3/2RRR3/8/8 w - - " 1)
add_test(movegen::king::move::blocked::b GenerateKingMoves    0  "8/8/8/2rrr3/2rkr3/2rrr3/8/8 b - - " 1)

add_test(movegen::king::move::attack::w GenerateKingMoves    3  "8/8/8/3R4/3Kr3/3R4/8/8 w - - " 0)
add_test(movegen::king::move::attack::b GenerateKingMoves    3  "8/8/8/8/2rkr3/3R4/8/8 b - - " 0)

add_test(movegen::king::castle::w GenerateKingMoves        7  "8/8/8/8/8/8/8/R3K2R w KQ - 0 1" 1)
add_test(movegen::king::castle::king::w GenerateKingMoves   6  "8/8/8/8/8/8/8/R3K2R w K - 0 1" 1)
add_test(movegen::king::castle::queen::w GenerateKingMoves  6  "8/8/8/8/8/8/8/R3K2R w Q - 0 1" 1)
add_test(movegen::king::castle::none::w GenerateKingMoves   5  "8/8/8/8/8/8/8/R3K2R w - - 0 1" 1)

add_test(movegen::king::castle::b GenerateKingMoves        7  "r3k2r/8/8/8/8/8/8/8 b kq - 0 1" 1)
add_test(movegen::king::castle::king::b GenerateKingMoves   6  "r3k2r/8/8/8/8/8/8/8 b k - 0 1" 1)
add_test(movegen::king::castle::queen::b GenerateKingMoves  6  "r3k2r/8/8/8/8/8/8/8 b q - 0 1" 1)
add_test(movegen::king::castle::none::b GenerateKingMoves   5  "r3k2r/8/8/8/8/8/8/8 b - - 0 1" 1)

add_test(movegen::king::castleblocked::w GenerateKingMoves         3  "8/8/8/8/8/8/8/R2QKQ1R w KQ - 0 1" 1)
add_test(movegen::king::castleblocked::king::w GenerateKingMoves    5  "8/8/8/8/8/8/8/RQ2K1QR w K - 0 1" 1)
add_test(movegen::king::castleblocked::queen::w GenerateKingMoves   5  "8/8/8/8/8/8/8/RQ2K1QR w Q - 0 1" 1)
add_test(movegen::king::castleblocked::none::w GenerateKingMoves    5  "8/8/8/8/8/8/8/RQ2K1QR w - - 0 1" 1)

add_test(movegen::king::castleblocked::b GenerateKingMoves         3  "r2qkq1r/8/8/8/8/8/8/8 b kq - 0 1" 1)
add_test(movegen::king::castleblocked::king::b GenerateKingMoves    5  "rq2k1qr/8/8/8/8/8/8/8 b k - 0 1" 1)
add_test(movegen::king::castleblocked::queen::b GenerateKingMoves   5  "rq2k1qr/8/8/8/8/8/8/8 b q - 0 1" 1)
add_test(movegen::king::castleblocked::none::b GenerateKingMoves    5  "rq2k1qr/8/8/8/8/8/8/8 b - - 0 1" 1)

add_test(movegen::king::attackedsquares::w GenerateKingMoves    1  "3r1r2/8/8/8/8/8/8/4K3 w - - 0 1" 1)
add_test(movegen::king::attackedsquares::b GenerateKingMoves    1  "3R1R2/8/8/8/8/8/8/4k3 b - - 0 1" 1)

add_test(movegen::king::attackedaquares::castle::1::w GenerateKingMoves    6  "6r1/8/8/8/8/8/8/R3K2R w KQ - 0 1" 1)
add_test(movegen::king::attackedaquares::castle::2::w GenerateKingMoves    4  "5r2/8/8/8/8/8/8/R3K2R w KQ - 0 1" 1)
add_test(movegen::king::attackedaquares::castle::3::w GenerateKingMoves    4  "3r4/8/8/8/8/8/8/R3K2R w KQ - 0 1" 1)
add_test(movegen::king::attackedaquares::castle::4::w GenerateKingMoves    6  "2r5/8/8/8/8/8/8/R3K2R w KQ - 0 1" 1)
add_test(movegen::king::attackedaquares::castle::5::w GenerateKingMoves    7  "1r6/8/8/8/8/8/8/R3K2R w KQ - 0 1" 1)

add_test(movegen::king::attackedaquares::castle::1::b GenerateKingMoves    6  "r3k2r/8/8/8/8/8/8/6R1 b kq - 0 1" 1)
add_test(movegen::king::attackedaquares::castle::2::b GenerateKingMoves    4  "r3k2r/8/8/8/8/8/8/5R2 b kq - 0 1" 1)
add_test(movegen::king::attackedaquares::castle::3::b GenerateKingMoves    4  "r3k2r/8/8/8/8/8/8/3R4 b kq - 0 1" 1)
add_test(movegen::king::attackedaquares::castle::4::b GenerateKingMoves    6  "r3k2r/8/8/8/8/8/8/2R5 b kq - 0 1" 1)
add_test(movegen::king::attackedaquares::castle::5::b GenerateKingMoves    7  "r3k2r/8/8/8/8/8/8/1R6 b kq - 0 1" 1)

add_test(movegen::king::kingsafety::move GenerateKingMoves   6   "3r4/8/8/8/3K4/8/8/8 w - - 0 1" 0)
add_test(movegen::king::kingsafety::capture GenerateKingMoves   7   "8/8/8/3k4/4B3/8/8/8 b - - 0 1" 0)
add_test(movegen::king::kingsafety::multi GenerateKingMoves   2   "r3k2r/p4ppp/1pnbB3/4p3/PpP5/3P1Pn1/8/3Kq3 w kq - 0 23" 0)
