#include "move_gen.hh"
#include "pawn_gen.hh"
#include "knight_gen.hh"
#include "bishop_gen.hh"
#include "rook_gen.hh"
#include "queen_gen.hh"
#include "king_gen.hh"


MoveGen::MoveGen(Color color) {
    pieceGen[(int) PieceType::King] = std::make_unique<KingGen>(KingGen(color));
    pieceGen[(int) PieceType::Queen] = std::make_unique<QueenGen>(QueenGen(color));
    pieceGen[(int) PieceType::Rook] = std::make_unique<RookGen>(RookGen(color));
    pieceGen[(int) PieceType::Bishop] = std::make_unique<BishopGen>(BishopGen(color));
    pieceGen[(int) PieceType::Knight] = std::make_unique<KnightGen>(KnightGen(color));
    pieceGen[(int) PieceType::Pawn] = std::make_unique<PawnGen>(PawnGen(color));
}

U8 MoveGen::GetAllMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board) {
    U8 moveCount = 0;
    for (const auto & gen : pieceGen)
        moveCount += gen->GetALlMoves(moves, board, moveCount);
    return moveCount;
}

U8 MoveGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board) {
    U8 moveCount = 0;
    for (const auto & gen : pieceGen)
        moveCount += gen->GetAttackMoves(moves, board, moveCount);
    return moveCount;
}

U8 MoveGen::GetQuietMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board) {
    U8 moveCount = 0;
    for (const auto & gen : pieceGen)
        moveCount += gen->GetQuietMoves(moves, board, moveCount);
    return moveCount;
}


