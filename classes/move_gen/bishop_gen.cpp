#include "bishop_gen.hh"

U8 BishopGen::GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Bishop);
    for (const auto & dir : bishopDirections)
        moveCount += GetSlidingMoves(moves, board, pieces, dir, isKingSafe, startIndex + moveCount, attackedSquares);

    return moveCount;
}

U8 BishopGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Bishop);
    for (const auto & dir : bishopDirections)
        moveCount += GetSlidingAttacks(moves, board, pieces, dir, isKingSafe, startIndex + moveCount, attackedSquares);

    return moveCount;
}


