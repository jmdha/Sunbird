#include "headers/queen_gen.hh"

U8 QueenGen::GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Queen);
    for (const auto & dir : queenDirections)
        moveCount += GetSlidingMoves(moves, board, pieces, dir, isKingSafe, startIndex + moveCount);

    return moveCount;
}

U8 QueenGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Queen);
    for (const auto & dir : queenDirections)
        moveCount += GetSlidingAttacks(moves, board, pieces, dir, isKingSafe, startIndex + moveCount);

    return moveCount;
}