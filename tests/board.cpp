#include "board.hpp"
#include "third_party/doctest.h"
#include "types.hpp"

TEST_SUITE("BOARD::CONSTRUCTOR") {
    TEST_CASE("DEFAULT") {
        const Board board = Board();
        CHECK_EQ(board.Turn(), WHITE);
        CHECK_EQ(board.MoveCount(), 0);
        CHECK_EQ(board.Ply(), 0);
        CHECK_EQ(board.EP(), SQUARE_NONE);
        CHECK_EQ(board.GetCastling(WHITE), Castling::All);
        CHECK_EQ(board.GetCastling(BLACK), Castling::All);
        CHECK_EQ(board.Pieces(), 0xffff00000000ffff);
        CHECK_EQ(board.Pieces(WHITE), 0xffff);
        CHECK_EQ(board.Pieces(BLACK), 0xffff000000000000);
        CHECK_EQ(board.Pieces(PAWN), 0xff00000000ff00);
        CHECK_EQ(board.Pieces(KNIGHT), 0x4200000000000042);
        CHECK_EQ(board.Pieces(BISHOP), 0x2400000000000024);
        CHECK_EQ(board.Pieces(QUEEN), 0x800000000000008);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
    }
    TEST_CASE("KIWIPETE") {
        const Board board =
            Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
        CHECK_EQ(board.Turn(), WHITE);
        CHECK_EQ(board.MoveCount(), 0);
        CHECK_EQ(board.Ply(), 0);
        CHECK_EQ(board.EP(), SQUARE_NONE);
        CHECK_EQ(board.GetCastling(WHITE), Castling::All);
        CHECK_EQ(board.GetCastling(BLACK), Castling::All);
        CHECK_EQ(board.Pieces(), 0x917d731812a4ff91);
        CHECK_EQ(board.Pieces(WHITE), 0x181024ff91);
        CHECK_EQ(board.Pieces(BLACK), 0x917d730002800000);
        CHECK_EQ(board.Pieces(PAWN), 0x2d50081280e700);
        CHECK_EQ(board.Pieces(KNIGHT), 0x221000040000);
        CHECK_EQ(board.Pieces(BISHOP), 0x40010000001800);
        CHECK_EQ(board.Pieces(QUEEN), 0x10000000200000);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
    }
}

TEST_SUITE("BOARD::UPDATE") {
    TEST_CASE("APPLY_MOVE") {
        Board board = Board();

        const uint64_t prior_hash = board.GetHash();
        board.ApplyMove(Move(D2, D4, Move::DoublePawnPush));
        CHECK_EQ(board.EP(), D3);
        CHECK_EQ(board.Pieces(PAWN), 0xff00000800f700);
        CHECK_EQ(board.Turn(), BLACK);
        CHECK_EQ(board.MoveCount(), 1);
        CHECK_EQ(board.Ply(), 1);
        CHECK_NE(board.GetHash(), prior_hash);
    }
    TEST_CASE("UNDO_MOVE") {
        Board board = Board();

        const uint64_t prior_hash = board.GetHash();
        board.ApplyMove(Move(D2, D4, Move::DoublePawnPush));
        CHECK_NE(board.GetHash(), prior_hash);
        board.UndoMove(Move(D2, D4, Move::DoublePawnPush));
        CHECK_EQ(board.Pieces(PAWN), 0xff00000000ff00);
        CHECK_EQ(board.Turn(), WHITE);
        CHECK_EQ(board.MoveCount(), 1);
        CHECK_EQ(board.Ply(), 0);
        CHECK_EQ(board.GetHash(), prior_hash);
    }
    TEST_CASE("PROMOTION") {
        Board board = Board("4k3/P7/8/8/8/8/8/4K3 w - - 0 1");

        const uint64_t prior_hash = board.GetHash();
        board.ApplyMove(Move(A7, A8, Move::QPromotion));
        CHECK_EQ(board.Pieces(PAWN), 0);
        CHECK_NE(board.GetHash(), prior_hash);
        CHECK_EQ(board.Pieces(QUEEN), 0x100000000000000);
        board.UndoMove(Move(A7, A8, Move::QPromotion));
        CHECK_EQ(board.Pieces(PAWN), 0x1000000000000);
        CHECK_EQ(board.Pieces(QUEEN), 0);
        CHECK_EQ(board.GetHash(), prior_hash);
    }
    TEST_CASE("CASTLING_KING_WHITE") {
        Board board = Board("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1");

        const uint64_t prior_hash = board.GetHash();
        CHECK_EQ(board.GetCastling(WHITE), Castling::All);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x81);
        CHECK_EQ(board.SquarePiece(E1), KING);
        CHECK_EQ(board.SquarePiece(A1), ROOK);
        CHECK_EQ(board.SquarePiece(H1), ROOK);
        board.ApplyMove(Move(E1, G1, Move::KingCastle));
        CHECK_NE(board.GetHash(), prior_hash);
        CHECK_EQ(board.GetCastling(WHITE), Castling::None);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000040);
        CHECK_EQ(board.Pieces(ROOK), 0x21);
        CHECK_EQ(board.SquarePiece(G1), KING);
        CHECK_EQ(board.SquarePiece(A1), ROOK);
        CHECK_EQ(board.SquarePiece(F1), ROOK);
        board.UndoMove(Move(E1, G1, Move::KingCastle));
        CHECK_EQ(board.GetCastling(WHITE), Castling::All);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x81);
        CHECK_EQ(board.SquarePiece(E1), KING);
        CHECK_EQ(board.SquarePiece(A1), ROOK);
        CHECK_EQ(board.SquarePiece(H1), ROOK);
        CHECK_EQ(board.GetHash(), prior_hash);
    }
    TEST_CASE("CASTLING_KING_BLACK") {
        Board board = Board("r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1");

        const uint64_t prior_hash = board.GetHash();
        CHECK_EQ(board.GetCastling(BLACK), Castling::All);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x8100000000000000);
        CHECK_EQ(board.SquarePiece(E8), KING);
        CHECK_EQ(board.SquarePiece(A8), ROOK);
        CHECK_EQ(board.SquarePiece(H8), ROOK);
        board.ApplyMove(Move(E8, G8, Move::KingCastle));
        CHECK_NE(board.GetHash(), prior_hash);
        CHECK_EQ(board.GetCastling(BLACK), Castling::None);
        CHECK_EQ(board.Pieces(KING), 0x4000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x2100000000000000);
        CHECK_EQ(board.SquarePiece(G8), KING);
        CHECK_EQ(board.SquarePiece(A8), ROOK);
        CHECK_EQ(board.SquarePiece(F8), ROOK);
        board.UndoMove(Move(E8, G8, Move::KingCastle));
        CHECK_EQ(board.GetCastling(BLACK), Castling::All);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x8100000000000000);
        CHECK_EQ(board.SquarePiece(E8), KING);
        CHECK_EQ(board.SquarePiece(A8), ROOK);
        CHECK_EQ(board.SquarePiece(H8), ROOK);
        CHECK_EQ(board.GetHash(), prior_hash);
    }
    TEST_CASE("CASTLING_QUEEN_WHITE") {
        Board board = Board("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1");

        const uint64_t prior_hash = board.GetHash();
        CHECK_EQ(board.GetCastling(WHITE), Castling::All);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x81);
        CHECK_EQ(board.SquarePiece(E1), KING);
        CHECK_EQ(board.SquarePiece(A1), ROOK);
        CHECK_EQ(board.SquarePiece(H1), ROOK);
        board.ApplyMove(Move(E1, C1, Move::QueenCastle));
        CHECK_NE(board.GetHash(), prior_hash);
        CHECK_EQ(board.GetCastling(WHITE), Castling::None);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000004);
        CHECK_EQ(board.Pieces(ROOK), 0x88);
        CHECK_EQ(board.SquarePiece(C1), KING);
        CHECK_EQ(board.SquarePiece(D1), ROOK);
        CHECK_EQ(board.SquarePiece(H1), ROOK);
        board.UndoMove(Move(E1, C1, Move::QueenCastle));
        CHECK_EQ(board.GetCastling(WHITE), Castling::All);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x81);
        CHECK_EQ(board.SquarePiece(E1), KING);
        CHECK_EQ(board.SquarePiece(A1), ROOK);
        CHECK_EQ(board.SquarePiece(H1), ROOK);
        CHECK_EQ(board.GetHash(), prior_hash);
    }
    TEST_CASE("CASTLING_QUEEN_BLACK") {
        Board board = Board("r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1");

        const uint64_t prior_hash = board.GetHash();
        CHECK_EQ(board.GetCastling(BLACK), Castling::All);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x8100000000000000);
        CHECK_EQ(board.SquarePiece(E8), KING);
        CHECK_EQ(board.SquarePiece(A8), ROOK);
        CHECK_EQ(board.SquarePiece(H8), ROOK);
        board.ApplyMove(Move(E8, C8, Move::QueenCastle));
        CHECK_NE(board.GetHash(), prior_hash);
        CHECK_EQ(board.GetCastling(BLACK), Castling::None);
        CHECK_EQ(board.Pieces(KING), 0x400000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x8800000000000000);
        CHECK_EQ(board.SquarePiece(C8), KING);
        CHECK_EQ(board.SquarePiece(D8), ROOK);
        CHECK_EQ(board.SquarePiece(H8), ROOK);
        board.UndoMove(Move(E8, C8, Move::QueenCastle));
        CHECK_EQ(board.GetCastling(BLACK), Castling::All);
        CHECK_EQ(board.Pieces(KING), 0x1000000000000010);
        CHECK_EQ(board.Pieces(ROOK), 0x8100000000000000);
        CHECK_EQ(board.SquarePiece(E8), KING);
        CHECK_EQ(board.SquarePiece(A8), ROOK);
        CHECK_EQ(board.SquarePiece(H8), ROOK);
        CHECK_EQ(board.GetHash(), prior_hash);
    }
    TEST_CASE("EP") {
        Board board = Board("8/8/8/8/1p6/8/P7/8 w - - 0 1", "a2a4");

        const uint64_t prior_hash = board.GetHash();
        CHECK_EQ(board.EP(), A3);
        CHECK_EQ(board.Pieces(PAWN), 0x3000000);
        board.ApplyMove(Move(B4, A3, Move::EPCapture));
        CHECK_NE(board.GetHash(), prior_hash);
        CHECK_EQ(board.Pieces(PAWN), 0x10000);
        board.UndoMove(Move(B4, A3, Move::EPCapture));
        CHECK_EQ(board.EP(), A3);
        CHECK_EQ(board.Pieces(PAWN), 0x3000000);
        CHECK_EQ(board.GetHash(), prior_hash);
    }
    TEST_CASE("THREEFOLD") {
        Board board =
            Board(FEN_START, "b1c3 b8c6 c3b1 c6b8 b1c3 b8c6 c3b1 c6b8 b1c3 b8c6 c3b1 c6b8");
        CHECK(board.IsThreefold());
    }
}
