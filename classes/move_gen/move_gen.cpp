#include "headers/move_gen.hh"
#include "headers/pawn_gen.hh"
#include "headers/knight_gen.hh"
#include "headers/bishop_gen.hh"
#include "headers/rook_gen.hh"
#include "headers/queen_gen.hh"
#include "headers/king_gen.hh"


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

void MoveGen::RemoveIllegal(std::array<Move, 128> *moves, U8 *moveCount, Board *board,
                            unsigned long long int attackedSquares) {
    for (U8 i = 0; i < *moveCount; i++) {
        if (IsIllegal(board, attackedSquares, moves->at(i))) {
            if (*moveCount != 1 && i != *moveCount - 1)
                moves->at(i) = moves->at(--*moveCount);
            else {
                moves->at(i) = Move();
                (*moveCount)--;
            }
        }
    }
}

bool MoveGen::IsIllegal(Board *board, unsigned long long int attackedSquares, Move move) {
    bool kingSafe;
    board->DoMove(move);
    board->SwitchTurn();
    kingSafe = board->IsKingSafe();
    board->SwitchTurn();
    board->UndoMove(move);
    return !kingSafe;
}
