#ifndef QUEEN_GEN_HH
#define QUEEN_GEN_HH

#include "piece_gen.hh"

class QueenGen : public PieceGen {
public:
    explicit QueenGen(Color color) : PieceGen(color) {};
    inline U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) final;
    inline U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) final;
};

inline U8 QueenGen::GetQuietMoves (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) {
    return GetQuietMovesGeneric(moves, board, PieceType::Queen, startIndex);
}

inline U8 QueenGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) {
    return GetAttackMovesGeneric(moves, board, PieceType::Queen, startIndex);
}

#endif //QUEEN_GEN_HH
