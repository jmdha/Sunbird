#include <chess/internal/bitboard.hpp>
#include <engine/evaluation.hpp>
#include <engine/internal/values.hpp>
#include <jank/bit/bit.hpp>

namespace Chess::Engine::Evaluation {
int Eval(const Position &pos) {
    std::array<int, 2> mg{0};
    std::array<int, 2> eg{0};
    int gamePhase = 0;

    for (auto pType : PIECES) {
        for (BB pieces = pos.GetPieces(pType); pieces;) {
            Square piece = (Square)jank::bit::lsb_pop(pieces);
            Color color = pos.GetColor(piece);
            mg[(int)color] += Values::MG[(int)color][(int)pType][(int)piece];
            eg[(int)color] += Values::EG[(int)color][(int)pType][(int)piece];
            gamePhase += Values::PHASE_INC[(int)pType];
        }
    }

    int mgScore = mg[(int)Color::White] - mg[(int)Color::Black];
    int egScore = eg[(int)Color::White] - eg[(int)Color::Black];
    int mgPhase = gamePhase;
    if (mgPhase > 24)
        mgPhase = 24;
    int egPhase = 24 - mgPhase;
    int value = (mgScore * mgPhase + egScore * egPhase) / 24;
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
