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

MoveGen::~MoveGen() {
    for (auto & gen : pieceGen)
        delete gen;
}

U8 MoveGen::GetAllMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares) {
    int moveCount = 0;
    bool isKingSafe = board->IsKingSafe();
    for (const auto & gen : pieceGen)
        moveCount += gen->GetALlMoves(moves, board, attackedSquares, isKingSafe, moveCount);
    return moveCount;
}

U8 MoveGen::GetAttackMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int attackedSquares) {
    int moveCount = 0;
    bool isKingSafe = board->IsKingSafe();
    for (const auto & gen : pieceGen)
        moveCount += gen->GetAttackMoves(moves, board, attackedSquares, isKingSafe, moveCount);
    return moveCount;
}
