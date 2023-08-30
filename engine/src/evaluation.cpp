#include "chess/internal/constants.hpp"
#include "chess/internal/utilities.hpp"
#include <chess/internal/bitboard.hpp>
#include <engine/evaluation.hpp>
#include <engine/internal/values.hpp>
#include <jank/bit/bit.hpp>

namespace Chess::Engine::Evaluation {
namespace Internal {
std::pair<int, int> CalculatePhase(const Position &pos) {
    int game_phase = 0;

    for (auto pType : PIECES) {
        const BB pieces = pos.GetPieces(pType);
        const int count = jank::bit::popcount(pieces);
        game_phase += count * Values::PHASE_INC[(int)pType];
    }

    const int mg = std::min(24, game_phase);
    const int eg = 24 - mg;
    return {mg, eg};
}

std::pair<int, int> EvalPieceSquare(const Position &pos, Color color) {
    int mg = 0;
    int eg = 0;

    for (auto pType : PIECES) {
        for (BB pieces = pos.GetPieces(color, pType); pieces;) {
            const Square piece = (Square)jank::bit::lsb_pop(pieces);
            mg += Values::MG[(int)color][(int)pType][(int)piece];
            eg += Values::EG[(int)color][(int)pType][(int)piece];
        }
    }

    return {mg, eg};
}

std::pair<int, int> EvalPieceSquare(const Position &pos) {
    const auto white = EvalPieceSquare(pos, Color::White);
    const auto black = EvalPieceSquare(pos, Color::Black);
    return {white.first - black.first, white.second - black.second};
}

std::pair<int, int> EvalPawn(const Position &pos, Color color) {
    static Direction UP[2] = {Direction::North, Direction::South};
    int mg = 0;
    int eg = 0;

    const BB PAWNS = pos.GetPieces(color, PieceType::Pawn);
    const BB PAWNS_O = pos.GetPieces(~color, PieceType::Pawn);

    // Doubled pawns
    for (auto column : COLUMNS)
        if (Multiple(PAWNS & (BB)column)) {
            mg += Values::Structure::DoubledPawn::MG;
            eg += Values::Structure::DoubledPawn::EG;
        }

    for (BB pawns = PAWNS; pawns;) {
        const Square pawn = (Square)jank::bit::lsb_pop(pawns);
        // Passed pawn check
        if (!(PawnPassMask(pawn, color) & PAWNS_O) && !(Ray(pawn, UP[(int)color]) & PAWNS)) {
            const int row = Utilities::GetRowIndex(pawn);
            mg += Values::Structure::PassedPawn::MG[(int)color][row];
            eg += Values::Structure::PassedPawn::EG[(int)color][row];
        }
        // Isolated pawn check
        if (!(PawnIsolationMask(pawn) & PAWNS)) {
            mg += Values::Structure::IsolatedPawn::MG;
            eg += Values::Structure::IsolatedPawn::EG;
        }
    }

    return {mg, eg};
}

std::pair<int, int> EvalPawn(const Position &pos) {
    const auto white = EvalPawn(pos, Color::White);
    const auto black = EvalPawn(pos, Color::Black);
    return {white.first - black.first, white.second - black.second};
}
} // namespace Internal

int Eval(const Position &pos) {
    const std::pair<int, int> game_phase = Internal::CalculatePhase(pos);

    int value = 0;

    const std::pair<int, int> piece_square = Internal::EvalPieceSquare(pos);
    value += game_phase.first * piece_square.first;
    value += game_phase.second * piece_square.second;

    const std::pair<int, int> pawn_structure = Internal::EvalPawn(pos);
    value += game_phase.first * pawn_structure.first;
    value += game_phase.second * pawn_structure.second;

    value /= 24;

    return (pos.GetTurn() == Color::White) ? value : -value;
}

int EvalNoMove(const Position &pos) {
    bool isKingSafe = pos.IsKingSafe();
    // Checkmate
    if (!isKingSafe)
        return -Values::INF;
    // Stalemate
    else
        return 0;
}
} // namespace Chess::Engine::Evaluation
