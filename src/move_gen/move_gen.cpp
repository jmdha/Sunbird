#include "move_gen.hh"
#include "pawn_gen.hh"
#include "knight_gen.hh"
#include "bishop_gen.hh"
#include "rook_gen.hh"
#include "queen_gen.hh"
#include "king_gen.hh"


MoveGen::MoveGen(Color color): color(color) {
    pieceGen[(int) PieceType::Pawn] = new PawnGen(color);
    pieceGen[(int) PieceType::Knight] = new KnightGen(color);
    pieceGen[(int) PieceType::Bishop] = new BishopGen(color);
    pieceGen[(int) PieceType::Rook] = new RookGen(color);
    pieceGen[(int) PieceType::Queen] = new QueenGen(color);
    pieceGen[(int) PieceType::King] = new KingGen(color);
}

U8 MoveGen::GetAllMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares) {
    U8 moveCount = 0;
    bool isKingSafe = board->IsKingSafe();
    for (const auto & gen : pieceGen)
        moveCount += gen->GetALlMoves(moves, board, attackedSquares, isKingSafe, moveCount);
    //RemoveIllegal(moves, &moveCount, board, attackedSquares);
    return moveCount;
}

U8 MoveGen::GetAttackMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int attackedSquares) {
    U8 moveCount = 0;
    bool isKingSafe = board->IsKingSafe();
    for (const auto & gen : pieceGen)
        moveCount += gen->GetAttackMoves(moves, board, attackedSquares, isKingSafe, moveCount);
    //RemoveIllegal(moves, &moveCount, board, attackedSquares);
    return moveCount;
}
