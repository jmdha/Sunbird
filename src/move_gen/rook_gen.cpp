#include "headers/rook_gen.hh"

U8 RookGen::GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Rook);
    for (const auto & dir : rookDirections)
        moveCount += GetSlidingMoves(moves, board, pieces, dir, isKingSafe, startIndex + moveCount, attackedSquares);

    return moveCount;
}

U8 RookGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Rook);
    for (const auto & dir : rookDirections)
        moveCount += GetSlidingAttacks(moves, board, pieces, dir, isKingSafe, startIndex + moveCount, attackedSquares);

    return moveCount;
}
