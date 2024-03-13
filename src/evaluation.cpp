#include "evaluation.hpp"
#include "bit.hpp"
#include "bitboard.hpp"
#include "types.hpp"
#include "utilities.hpp"
#include "values.hpp"

namespace Evaluation {
namespace {
std::pair<int, int> CalculatePhase(const Board &board) {
    int game_phase = 0;

    for (auto pType : PIECES) {
        const BB pieces = board.Pieces(pType);
        const int count = popcount(pieces);
        game_phase += count * Values::PHASE_INC[static_cast<int>(pType)];
    }

    const int mg = std::min(24, game_phase);
    const int eg = 24 - mg;
    return {mg, eg};
}

template <Color color>
std::pair<int, int> EvalPieceSquare(const Board &board) {
    const size_t colorI = static_cast<size_t>(color);
    int mg              = 0;
    int eg              = 0;

    for (auto pType : PIECES) {
        const size_t pieceI = static_cast<size_t>(pType);
        for (BB pieces = board.Pieces(color, pType); pieces;) {
            const Square piece = static_cast<Square>(lsb_pop(pieces));
            mg += Values::MG[colorI][pieceI][static_cast<size_t>(piece)];
            eg += Values::EG[colorI][pieceI][static_cast<size_t>(piece)];
        }
    }

    return {mg, eg};
}

std::pair<int, int> EvalPieceSquare(const Board &board) {
    const auto white = EvalPieceSquare<WHITE>(board);
    const auto black = EvalPieceSquare<BLACK>(board);
    return {white.first - black.first, white.second - black.second};
}

template <Color color>
std::pair<int, int> EvalPawn(const Board &board) {
    const size_t colorI    = static_cast<size_t>(color);
    static Direction UP[2] = {NORTH, SOUTH};
    int mg                 = 0;
    int eg                 = 0;

    const BB PAWNS   = board.Pieces(color, PAWN);
    const BB PAWNS_O = board.Pieces(~color, PAWN);

    // Doubled pawns
    for (auto column : COLUMNS)
        if (Multiple(PAWNS & static_cast<BB>(column))) {
            mg += Values::Structure::DoubledPawn::MG;
            eg += Values::Structure::DoubledPawn::EG;
        }

    for (BB pawns = PAWNS; pawns;) {
        const Square pawn = static_cast<Square>(lsb_pop(pawns));
        // Passed pawn check
        if (!(PawnPassMask(pawn, color) & PAWNS_O) && !(Ray(pawn, UP[colorI]) & PAWNS)) {
            const int row = Utilities::GetRowIndex(pawn);
            mg += Values::Structure::PassedPawn::MG[colorI][static_cast<size_t>(row)];
            eg += Values::Structure::PassedPawn::EG[colorI][static_cast<size_t>(row)];
        }
        // Isolated pawn check
        if (!(PawnIsolationMask(pawn) & PAWNS)) {
            mg += Values::Structure::IsolatedPawn::MG;
            eg += Values::Structure::IsolatedPawn::EG;
        }
    }

    return {mg, eg};
}

std::pair<int, int> EvalPawn(const Board &board) {
    const auto white = EvalPawn<WHITE>(board);
    const auto black = EvalPawn<BLACK>(board);
    return {white.first - black.first, white.second - black.second};
}
} // namespace

int Eval(const Board &board) {
    const std::pair<int, int> game_phase = CalculatePhase(board);

    int value = 0;

    const std::pair<int, int> piece_square = EvalPieceSquare(board);
    value += game_phase.first * piece_square.first;
    value += game_phase.second * piece_square.second;

    const std::pair<int, int> pawn_structure = EvalPawn(board);
    value += game_phase.first * pawn_structure.first;
    value += game_phase.second * pawn_structure.second;

    value /= 24;

    return (board.Turn() == WHITE) ? value : -value;
}

int EvalNoMove(const Board &board) {
    bool isKingSafe = board.IsKingSafe(board.Turn());
    // Checkmate
    if (!isKingSafe) return -Values::INF;
    // Stalemate
    else
        return 0;
}
} // namespace Evaluation
