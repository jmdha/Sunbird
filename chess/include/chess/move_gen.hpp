#ifndef MOVE_GEN
#define MOVE_GEN

#include <memory>
#include <strings.h>
#include <vector>

#include "board.hpp"
#include "internal/bit_shift.hpp"
#include "internal/constants.hpp"
#include "internal/move.hpp"
#include "internal/move_list.hpp"

namespace MoveGen {
enum class GenType { Quiet, Attack, All };

// Generates moves for an individual piece type
template <GenType, PieceType> void Generate(const Board &board, Color color, MoveList &moves);

// Generates moves for all pieces
template <GenType gType> MoveList GenerateMoves(const Board &board, Color color) {
    MoveList moves;
    Generate<gType, PieceType::Pawn>(board, color, moves);
    Generate<gType, PieceType::Knight>(board, color, moves);
    Generate<gType, PieceType::Bishop>(board, color, moves);
    Generate<gType, PieceType::Rook>(board, color, moves);
    Generate<gType, PieceType::Queen>(board, color, moves);
    Generate<gType, PieceType::King>(board, color, moves);
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
}; // namespace MoveGen

#endif
