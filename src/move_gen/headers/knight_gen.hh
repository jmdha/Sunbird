#ifndef KNIGHT_GEN_HH
#define KNIGHT_GEN_HH

#include "piece_gen.hh"

class KnightGen : public PieceGen {
public:
    explicit KnightGen(Color color) : PieceGen(color) {};
    U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
    U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
};


#endif //KNIGHT_GEN_HH
