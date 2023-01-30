#ifndef PAWN_GEN_HH
#define PAWN_GEN_HH

#include "piece_gen.hh"

class PawnGen : public PieceGen {
public:
    explicit PawnGen(Color color) : PieceGen(color),
    up((color == Color::White) ? Direction::North : Direction::South),
    down((color == Color::White) ? Direction::South : Direction::North),
    doubleRank((color == Color::White) ? Row::Row2 : Row::Row7),
    enPassantRank((color == Color::White) ? Row::Row6 : Row::Row3),
    promotionRank((color == Color::White) ? Row::Row7 : Row::Row2),
    notPromotionRank((color == Color::White) ? NotEdge::North : NotEdge::South) {
        GeneratePawnMoves();
    }
    U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
    U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
private:
    Direction up;
    Direction down;
    Row doubleRank;
    Row enPassantRank;
    Row promotionRank;
    NotEdge notPromotionRank;
    U64 singleMove[64] = { 0 };
    U64 doubleMove[64] = { 0 };

    void GeneratePawnMoves();
};


#endif //PAWN_GEN_HH
