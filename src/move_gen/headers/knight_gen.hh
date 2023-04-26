#ifndef KNIGHT_GEN_HH
#define KNIGHT_GEN_HH

#include "piece_gen.hh"

class KnightGen : public PieceGen {
public:
    explicit KnightGen(Color color) : PieceGen(color) {};
    inline U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
    inline U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
};

U8 KnightGen::GetQuietMoves (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    return GetQuietMovesGeneric(moves, board, PieceType::Knight, isKingSafe, startIndex, attackedSquares);
}

U8 KnightGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    return GetAttackMovesGeneric(moves, board, PieceType::Knight, isKingSafe, startIndex, attackedSquares);
}

#endif //KNIGHT_GEN_HH
