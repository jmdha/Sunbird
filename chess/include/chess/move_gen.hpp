#ifndef CHESS_MOVE_GEN
#define CHESS_MOVE_GEN

#include <memory>
#include <string>
#include <vector>

#include "board.hpp"
#include "internal/bit_shift.hpp"
#include "internal/constants.hpp"
#include "internal/move.hpp"
#include "internal/move_list.hpp"

namespace Chess::MoveGen {
enum class GenType { Quiet, Attack, All };

// Generates moves for an individual piece type
template <GenType, PieceType> void Generate(const Board &board, Color color, MoveList &moves);

// Generates moves for all pieces
template <GenType gType> MoveList GenerateMoves(const Board &board, Color color) {
    MoveList moves;
    if constexpr (gType == GenType::All || gType == GenType::Attack) {
        Generate<GenType::Attack, PieceType::Pawn>(board, color, moves);
        Generate<GenType::Attack, PieceType::Knight>(board, color, moves);
        Generate<GenType::Attack, PieceType::Bishop>(board, color, moves);
        Generate<GenType::Attack, PieceType::Rook>(board, color, moves);
        Generate<GenType::Attack, PieceType::Queen>(board, color, moves);
        Generate<GenType::Attack, PieceType::King>(board, color, moves);
    }
    if constexpr (gType == GenType::All || gType == GenType::Quiet) {
        Generate<GenType::Quiet, PieceType::Pawn>(board, color, moves);
        Generate<GenType::Quiet, PieceType::Knight>(board, color, moves);
        Generate<GenType::Quiet, PieceType::Bishop>(board, color, moves);
        Generate<GenType::Quiet, PieceType::Rook>(board, color, moves);
        Generate<GenType::Quiet, PieceType::Queen>(board, color, moves);
        Generate<GenType::Quiet, PieceType::King>(board, color, moves);
    }
    return moves;
}

// Generates moves for all pieces of type
template <GenType gType> MoveList GenerateMoves(const Board &board, Color color, PieceType pType) {
    assert(pType != PieceType::None);
    MoveList moves;
    switch (pType) {
    case PieceType::Pawn:
        Generate<gType, PieceType::Pawn>(board, color, moves);
        break;
    case PieceType::Knight:
        Generate<gType, PieceType::Knight>(board, color, moves);
        break;
    case PieceType::Bishop:
        Generate<gType, PieceType::Bishop>(board, color, moves);
        break;
    case PieceType::Rook:
        Generate<gType, PieceType::Rook>(board, color, moves);
        break;
    case PieceType::Queen:
        Generate<gType, PieceType::Queen>(board, color, moves);
        break;
    case PieceType::King:
        Generate<gType, PieceType::King>(board, color, moves);
        break;
    case PieceType::None:
        break;
    }
    return moves;
}
}; // namespace Chess::MoveGen

#endif
