#ifndef BISHOP_GEN_HH
#define BISHOP_GEN_HH

#include "piece_gen.hh"

class BishopGen : public PieceGen {
public:
    explicit BishopGen(Color color) : PieceGen(color) {};
    U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
    U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
};


#endif //BISHOP_GEN_HH
