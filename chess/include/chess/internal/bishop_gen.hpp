#ifndef BISHOP_GEN_HH
#define BISHOP_GEN_HH

#include "piece_gen.hpp"

class BishopGen : public PieceGen {
public:
    explicit BishopGen(Color color) : PieceGen(color) {};
    inline U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) final;
    inline U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) final;
};

inline U8 BishopGen::GetQuietMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) {
    return GetQuietMovesGeneric(moves, board, PieceType::Bishop, startIndex);
}

inline U8 BishopGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) {
    return GetAttackMovesGeneric(moves, board, PieceType::Bishop, startIndex);
}

#endif //BISHOP_GEN_HH
