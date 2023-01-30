#ifndef QUEEN_GEN_HH
#define QUEEN_GEN_HH

#include "piece_gen.hh"

class QueenGen : public PieceGen {
public:
    explicit QueenGen(Color color) : PieceGen(color) {};
    U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
    inline U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
};


#endif //QUEEN_GEN_HH
