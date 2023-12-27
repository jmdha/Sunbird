#ifndef CHESS_MOVE_GEN
#define CHESS_MOVE_GEN

#include <memory>
#include <string>
#include <vector>

#include "internal/constants.hpp"
#include "internal/move.hpp"
#include "internal/move_list.hpp"
#include "internal/position.hpp"

namespace Chess::MoveGen {
enum class GenType { Quiet, Attack, All };

// Generates moves for an individual piece type
template <GenType, PieceType> void Generate(const Position &pos, Color color, MoveList &moves);

// Generates moves for all pieces
template <GenType gType> MoveList GenerateMoves(const Position &pos, Color color) {
    MoveList moves;
    if constexpr (gType == GenType::All) {
        Generate<GenType::All, PieceType::Pawn>(pos, color, moves);
        Generate<GenType::All, PieceType::Knight>(pos, color, moves);
        Generate<GenType::All, PieceType::Bishop>(pos, color, moves);
        Generate<GenType::All, PieceType::Rook>(pos, color, moves);
        Generate<GenType::All, PieceType::Queen>(pos, color, moves);
        Generate<GenType::All, PieceType::King>(pos, color, moves);
    }
    if constexpr (gType == GenType::Attack) {
        Generate<GenType::Attack, PieceType::Pawn>(pos, color, moves);
        Generate<GenType::Attack, PieceType::Knight>(pos, color, moves);
        Generate<GenType::Attack, PieceType::Bishop>(pos, color, moves);
        Generate<GenType::Attack, PieceType::Rook>(pos, color, moves);
        Generate<GenType::Attack, PieceType::Queen>(pos, color, moves);
        Generate<GenType::Attack, PieceType::King>(pos, color, moves);
    }
    if constexpr (gType == GenType::Quiet) {
        Generate<GenType::Quiet, PieceType::Pawn>(pos, color, moves);
        Generate<GenType::Quiet, PieceType::Knight>(pos, color, moves);
        Generate<GenType::Quiet, PieceType::Bishop>(pos, color, moves);
        Generate<GenType::Quiet, PieceType::Rook>(pos, color, moves);
        Generate<GenType::Quiet, PieceType::Queen>(pos, color, moves);
        Generate<GenType::Quiet, PieceType::King>(pos, color, moves);
    }
    moves.finish();
    return moves;
}
template <GenType gType = GenType::All> inline MoveList GenerateMoves(const Position &pos) {
    return GenerateMoves<gType>(pos, pos.GetTurn());
}

// Generates moves for all pieces of type
template <GenType gType> MoveList GenerateMoves(const Position &pos, Color color, PieceType pType) {
    assert(pType != PieceType::None);
    MoveList moves;
    switch (pType) {
    case PieceType::Pawn:
        Generate<gType, PieceType::Pawn>(pos, color, moves);
        break;
    case PieceType::Knight:
        Generate<gType, PieceType::Knight>(pos, color, moves);
        break;
    case PieceType::Bishop:
        Generate<gType, PieceType::Bishop>(pos, color, moves);
        break;
    case PieceType::Rook:
        Generate<gType, PieceType::Rook>(pos, color, moves);
        break;
    case PieceType::Queen:
        Generate<gType, PieceType::Queen>(pos, color, moves);
        break;
    case PieceType::King:
        Generate<gType, PieceType::King>(pos, color, moves);
        break;
    case PieceType::None:
        break;
    }
    moves.finish();
    return moves;
}
} // namespace Chess::MoveGen

#endif
