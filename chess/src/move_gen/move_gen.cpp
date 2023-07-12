#include "chess/move_gen.hpp"
#include "chess/internal/pawn_gen.hpp"
#include "chess/internal/knight_gen.hpp"
#include "chess/internal/bishop_gen.hpp"
#include "chess/internal/rook_gen.hpp"
#include "chess/internal/queen_gen.hpp"
#include "chess/internal/king_gen.hpp"


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


