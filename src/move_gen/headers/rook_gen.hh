#ifndef ROOK_GEN_HH
#define ROOK_GEN_HH

#include "piece_gen.hh"

class RookGen : public PieceGen {
public:
    explicit RookGen(Color color) : PieceGen(color) {};
    U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
    inline U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
};


#endif //ROOK_GEN_HH
