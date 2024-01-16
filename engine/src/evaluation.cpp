#include <chess/internal/bit.hpp>
#include <chess/internal/bitboard.hpp>
#include <chess/internal/types.hpp>
#include <chess/internal/utilities.hpp>
#include <engine/evaluation.hpp>
#include <engine/internal/values.hpp>

namespace Chess::Engine::Evaluation {
namespace {
std::pair<int, int> CalculatePhase(const Position &pos) {
    int game_phase = 0;

    for (auto pType : PIECES) {
        const BB pieces = pos.GetPieces(pType);
        const int count = Bit::popcount(pieces);
        game_phase += count * Values::PHASE_INC[static_cast<int>(pType)];
    }

    const int mg = std::min(24, game_phase);
    const int eg = 24 - mg;
    return {mg, eg};
}

template <Color color> std::pair<int, int> EvalPieceSquare(const Position &pos) {
    const size_t colorI = static_cast<size_t>(color);
    int mg = 0;
    int eg = 0;

    for (auto pType : PIECES) {
        const size_t pieceI = static_cast<size_t>(pType);
        for (BB pieces = pos.GetPieces(color, pType); pieces;) {
            const Square piece = static_cast<Square>(Bit::lsb_pop(pieces));
            mg += Values::MG[colorI][pieceI][static_cast<size_t>(piece)];
            eg += Values::EG[colorI][pieceI][static_cast<size_t>(piece)];
        }
    }

    return {mg, eg};
}

std::pair<int, int> EvalPieceSquare(const Position &pos) {
    const auto white = EvalPieceSquare<Color::White>(pos);
    const auto black = EvalPieceSquare<Color::Black>(pos);
    return {white.first - black.first, white.second - black.second};
}

template <Color color> std::pair<int, int> EvalPawn(const Position &pos) {
    const size_t colorI = static_cast<size_t>(color);
    static Direction UP[2] = {Direction::North, Direction::South};
    int mg = 0;
    int eg = 0;

    const BB PAWNS = pos.GetPieces(color, PieceType::Pawn);
    const BB PAWNS_O = pos.GetPieces(~color, PieceType::Pawn);

    // Doubled pawns
    for (auto column : COLUMNS)
        if (Multiple(PAWNS & static_cast<BB>(column))) {
            mg += Values::Structure::DoubledPawn::MG;
            eg += Values::Structure::DoubledPawn::EG;
        }

    for (BB pawns = PAWNS; pawns;) {
        const Square pawn = static_cast<Square>(Bit::lsb_pop(pawns));
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

std::pair<int, int> EvalPawn(const Position &pos) {
    const auto white = EvalPawn<Color::White>(pos);
    const auto black = EvalPawn<Color::Black>(pos);
    return {white.first - black.first, white.second - black.second};
}
} // namespace

int Eval(const Position &pos) {
    const std::pair<int, int> game_phase = CalculatePhase(pos);

    int value = 0;

    const std::pair<int, int> piece_square = EvalPieceSquare(pos);
    value += game_phase.first * piece_square.first;
    value += game_phase.second * piece_square.second;

    const std::pair<int, int> pawn_structure = EvalPawn(pos);
    value += game_phase.first * pawn_structure.first;
    value += game_phase.second * pawn_structure.second;

    value /= 24;

    return (pos.GetTurn() == Color::White) ? value : -value;
}

int EvalNoMove(const Position &pos) {
    bool isKingSafe = pos.IsKingSafe(pos.GetTurn());
    // Checkmate
    if (!isKingSafe)
        return -Values::INF;
    // Stalemate
    else
        return 0;
}
} // namespace Chess::Engine::Evaluation
