#include "move.hpp"
#include "third_party/doctest.h"
#include "types.hpp"

TEST_CASE("MOVE::EXPORT") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).Export(), "a1a8");
    CHECK_EQ(Move(A8, A1, Move::Quiet).Export(), "a8a1");
    CHECK_EQ(Move(H1, H8, Move::Quiet).Export(), "h1h8");
    CHECK_EQ(Move(H8, H1, Move::Quiet).Export(), "h8h1");

    CHECK_EQ(Move(A1, H1, Move::Quiet).Export(), "a1h1");
    CHECK_EQ(Move(H1, A1, Move::Quiet).Export(), "h1a1");
    CHECK_EQ(Move(A8, H8, Move::Quiet).Export(), "a8h8");
    CHECK_EQ(Move(H8, A8, Move::Quiet).Export(), "h8a8");

    CHECK_EQ(Move(B7, B8, Move::NPromotion).Export(), "b7b8n");
    CHECK_EQ(Move(C7, C8, Move::BPromotion).Export(), "c7c8b");
    CHECK_EQ(Move(D7, D8, Move::RPromotion).Export(), "d7d8r");
    CHECK_EQ(Move(E7, E8, Move::QPromotion).Export(), "e7e8q");
    CHECK_EQ(Move(F7, G8, Move::NPromotionCapture).Export(), "f7g8n");
    CHECK_EQ(Move(G7, F8, Move::BPromotionCapture).Export(), "g7f8b");
    CHECK_EQ(Move(F7, H8, Move::RPromotionCapture).Export(), "f7h8r");
    CHECK_EQ(Move(H7, F8, Move::QPromotionCapture).Export(), "h7f8q");
}

TEST_CASE("MOVE::ORIGIN") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).Origin(), A1);
    CHECK_EQ(Move(A1, H1, Move::Quiet).Origin(), A1);
    CHECK_EQ(Move(A1, H1, Move::Capture).Origin(), A1);
    CHECK_EQ(Move(H8, A8, Move::Quiet).Origin(), H8);
    CHECK_EQ(Move(H8, H1, Move::Quiet).Origin(), H8);
    CHECK_EQ(Move(H8, H1, Move::Capture).Origin(), H8);
}

TEST_CASE("MOVE::DESTINATION") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).Destination(), A8);
    CHECK_EQ(Move(A1, H1, Move::Quiet).Destination(), H1);
    CHECK_EQ(Move(A1, H1, Move::Capture).Destination(), H1);
    CHECK_EQ(Move(H8, A8, Move::Quiet).Destination(), A8);
    CHECK_EQ(Move(H8, H1, Move::Quiet).Destination(), H1);
    CHECK_EQ(Move(H8, H1, Move::Capture).Destination(), H1);
}

TEST_CASE("MOVE::ISCAPTURE") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).IsCapture(), false);
    CHECK_EQ(Move(A1, A8, Move::DoublePawnPush).IsCapture(), false);
    CHECK_EQ(Move(A1, A8, Move::KingCastle).IsCapture(), false);
    CHECK_EQ(Move(A1, A8, Move::QueenCastle).IsCapture(), false);
    CHECK_EQ(Move(A1, A8, Move::Capture).IsCapture(), true);
    CHECK_EQ(Move(A1, A8, Move::EPCapture).IsCapture(), true);
    CHECK_EQ(Move(A1, A8, Move::NPromotion).IsCapture(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotion).IsCapture(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotion).IsCapture(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotion).IsCapture(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotionCapture).IsCapture(), true);
    CHECK_EQ(Move(A1, A8, Move::BPromotionCapture).IsCapture(), true);
    CHECK_EQ(Move(A1, A8, Move::RPromotionCapture).IsCapture(), true);
    CHECK_EQ(Move(A1, A8, Move::QPromotionCapture).IsCapture(), true);
}

TEST_CASE("MOVE::ISPROMOTION") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).IsPromotion(), false);
    CHECK_EQ(Move(A1, A8, Move::DoublePawnPush).IsPromotion(), false);
    CHECK_EQ(Move(A1, A8, Move::KingCastle).IsPromotion(), false);
    CHECK_EQ(Move(A1, A8, Move::QueenCastle).IsPromotion(), false);
    CHECK_EQ(Move(A1, A8, Move::Capture).IsPromotion(), false);
    CHECK_EQ(Move(A1, A8, Move::EPCapture).IsPromotion(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotion).IsPromotion(), true);
    CHECK_EQ(Move(A1, A8, Move::BPromotion).IsPromotion(), true);
    CHECK_EQ(Move(A1, A8, Move::RPromotion).IsPromotion(), true);
    CHECK_EQ(Move(A1, A8, Move::QPromotion).IsPromotion(), true);
    CHECK_EQ(Move(A1, A8, Move::NPromotionCapture).IsPromotion(), true);
    CHECK_EQ(Move(A1, A8, Move::BPromotionCapture).IsPromotion(), true);
    CHECK_EQ(Move(A1, A8, Move::RPromotionCapture).IsPromotion(), true);
    CHECK_EQ(Move(A1, A8, Move::QPromotionCapture).IsPromotion(), true);
}

TEST_CASE("MOVE::ISENPASSANT") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::DoublePawnPush).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::KingCastle).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::QueenCastle).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::Capture).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::EPCapture).IsEnPassant(), true);
    CHECK_EQ(Move(A1, A8, Move::NPromotion).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotion).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotion).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotion).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotionCapture).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotionCapture).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotionCapture).IsEnPassant(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotionCapture).IsEnPassant(), false);
}

TEST_CASE("MOVE::ISDOUBLE") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::DoublePawnPush).IsDouble(), true);
    CHECK_EQ(Move(A1, A8, Move::KingCastle).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::QueenCastle).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::Capture).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::EPCapture).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotion).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotion).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotion).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotion).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotionCapture).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotionCapture).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotionCapture).IsDouble(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotionCapture).IsDouble(), false);
}

TEST_CASE("MOVE::ISKINGCASTLE") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::DoublePawnPush).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::KingCastle).IsKingCastle(), true);
    CHECK_EQ(Move(A1, A8, Move::QueenCastle).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::Capture).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::EPCapture).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotion).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotion).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotion).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotion).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotionCapture).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotionCapture).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotionCapture).IsKingCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotionCapture).IsKingCastle(), false);
}

TEST_CASE("MOVE::ISQUEENCASTLE") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::DoublePawnPush).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::KingCastle).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::QueenCastle).IsQueenCastle(), true);
    CHECK_EQ(Move(A1, A8, Move::Capture).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::EPCapture).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotion).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotion).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotion).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotion).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotionCapture).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotionCapture).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotionCapture).IsQueenCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotionCapture).IsQueenCastle(), false);
}

TEST_CASE("MOVE::ISCASTLE") {
    CHECK_EQ(Move(A1, A8, Move::Quiet).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::DoublePawnPush).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::KingCastle).IsCastle(), true);
    CHECK_EQ(Move(A1, A8, Move::QueenCastle).IsCastle(), true);
    CHECK_EQ(Move(A1, A8, Move::Capture).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::EPCapture).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotion).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotion).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotion).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotion).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::NPromotionCapture).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::BPromotionCapture).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::RPromotionCapture).IsCastle(), false);
    CHECK_EQ(Move(A1, A8, Move::QPromotionCapture).IsCastle(), false);
}

TEST_CASE("MOVE::PROMOTIONPIECE") {
    CHECK_EQ(Move(A1, A8, Move::NPromotion).PromotionPiece(), KNIGHT);
    CHECK_EQ(Move(A1, A8, Move::BPromotion).PromotionPiece(), BISHOP);
    CHECK_EQ(Move(A1, A8, Move::RPromotion).PromotionPiece(), ROOK);
    CHECK_EQ(Move(A1, A8, Move::QPromotion).PromotionPiece(), QUEEN);
    CHECK_EQ(Move(A1, A8, Move::NPromotionCapture).PromotionPiece(), KNIGHT);
    CHECK_EQ(Move(A1, A8, Move::BPromotionCapture).PromotionPiece(), BISHOP);
    CHECK_EQ(Move(A1, A8, Move::RPromotionCapture).PromotionPiece(), ROOK);
    CHECK_EQ(Move(A1, A8, Move::QPromotionCapture).PromotionPiece(), QUEEN);
}
