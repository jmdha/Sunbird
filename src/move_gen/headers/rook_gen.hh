#ifndef ROOK_GEN_HH
#define ROOK_GEN_HH

#include "piece_gen.hh"

class RookGen : public PieceGen {
public:
    explicit RookGen(Color color) : PieceGen(color) {};
    inline U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) final;
    inline U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) final;
};

U8 RookGen::GetQuietMoves (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) {
    return GetQuietMovesGeneric(moves, board, PieceType::Rook, startIndex);
}

U8 RookGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) {
    return GetAttackMovesGeneric(moves, board, PieceType::Rook, startIndex);
}


#endif //ROOK_GEN_HH
