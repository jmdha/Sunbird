#include "move_gen.hpp"
#include "move.hpp"
#include "third_party/doctest.h"
#include "types.hpp"

TEST_SUITE("MOVE_GEN") {
    TEST_CASE("STARTPOS") {
        Board board    = Board();
        MoveList moves = GenerateMovesAll(board, WHITE);
        CHECK_EQ(moves.size(), 20);
        CHECK(moves.contains(Move(A2, A3, Move::Quiet)));
        CHECK(moves.contains(Move(A2, A4, Move::DoublePawnPush)));
        CHECK(moves.contains(Move(B2, B3, Move::Quiet)));
        CHECK(moves.contains(Move(B2, B4, Move::DoublePawnPush)));
        CHECK(moves.contains(Move(C2, C3, Move::Quiet)));
        CHECK(moves.contains(Move(C2, C4, Move::DoublePawnPush)));
        CHECK(moves.contains(Move(D2, D3, Move::Quiet)));
        CHECK(moves.contains(Move(D2, D4, Move::DoublePawnPush)));
        CHECK(moves.contains(Move(E2, E3, Move::Quiet)));
        CHECK(moves.contains(Move(E2, E4, Move::DoublePawnPush)));
        CHECK(moves.contains(Move(F2, F3, Move::Quiet)));
        CHECK(moves.contains(Move(F2, F4, Move::DoublePawnPush)));
        CHECK(moves.contains(Move(G2, G3, Move::Quiet)));
        CHECK(moves.contains(Move(G2, G4, Move::DoublePawnPush)));
        CHECK(moves.contains(Move(H2, H3, Move::Quiet)));
        CHECK(moves.contains(Move(H2, H4, Move::DoublePawnPush)));
    }
    TEST_CASE("PAWN::CAPTURE") {
        Board board    = Board("rnbqkbnr/pppppp1p/8/6p1/7P/8/PPPPPPP1/RNBQKBNR w KQkq - 0 1");
        MoveList moves = GenerateMovesAll(board, WHITE);
        CHECK(moves.contains(Move(H4, G5, Move::Capture)));
    }
}
