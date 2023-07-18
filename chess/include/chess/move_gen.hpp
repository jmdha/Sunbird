#ifndef MOVE_GEN
#define MOVE_GEN

#include <memory>
#include <strings.h>
#include <vector>

#include "board.hpp"
#include "internal/bit_shift.hpp"
#include "internal/constants.hpp"
#include "internal/move_list.hpp"
#include "internal/move.hpp"

namespace MoveGen {
enum class GenType { Quiet, Attack, All };

// Generates moves for an individual piece type
template <GenType, PieceType>
MoveList Generate(const Board &board);

// Generates moves for all pieces
template <GenType gType> MoveList GenerateMoves(const Board &board) {
    MoveList moves;
    moves << Generate<gType, PieceType::Pawn>(board);
    moves << Generate<gType, PieceType::Knight>(board);
    moves << Generate<gType, PieceType::Bishop>(board);
    moves << Generate<gType, PieceType::Rook>(board);
    moves << Generate<gType, PieceType::Queen>(board);
    moves << Generate<gType, PieceType::King>(board);
    return moves;
}
}; // namespace MoveGen

#endif
