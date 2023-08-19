#include <chess/internal/bitboard.hpp>
#include <engine/evaluation.hpp>
#include <engine/internal/values.hpp>
#include <jank/bit/bit.hpp>

namespace Chess::Engine::Evaluation {
int Eval(const Position &pos) {
    int mg_white = 0;
    int mg_black = 0;
    int eg_white = 0;
    int eg_black = 0;
    int gamePhase = 0;

    for (auto pType : PIECES) {
        for (BB pieces = pos.GetPieces(Color::White, pType); pieces;) {
            const Square piece = (Square)jank::bit::lsb_pop(pieces);
            mg_white += Values::MG[(int)Color::White][(int)pType][(int)piece];
            eg_white += Values::EG[(int)Color::White][(int)pType][(int)piece];
            gamePhase += Values::PHASE_INC[(int)pType];
        }
        for (BB pieces = pos.GetPieces(Color::Black, pType); pieces;) {
            const Square piece = (Square)jank::bit::lsb_pop(pieces);
            mg_black += Values::MG[(int)Color::Black][(int)pType][(int)piece];
            eg_black += Values::EG[(int)Color::Black][(int)pType][(int)piece];
            gamePhase += Values::PHASE_INC[(int)pType];
        }
    }

    const int mgScore = mg_white - mg_black;
    const int egScore = eg_white - eg_black;
    const int mgPhase = std::min(24, gamePhase);
    const int egPhase = 24 - mgPhase;
    const int value = (mgScore * mgPhase + egScore * egPhase) / 24;
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
