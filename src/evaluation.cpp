#include "evaluation.hpp"
#include "bb.hpp"
#include "masks.hpp"
#include "values.hpp"

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
std::pair<int, int> EvalPieceSquare(const Board &board, Color turn) {
    int mg = 0;
    int eg = 0;

    for (const auto type : PIECES) {
        for (BB pieces = board.Pieces(turn, type); pieces;) {
            const Square piece = lsb_pop(pieces);
            mg += Values::MG[turn][type][piece];
            mg += Values::EG[turn][type][piece];
        }
    }

    return {mg, eg};
}

std::pair<int, int> EvalPieceSquare(const Board &board) {
    const auto us  = EvalPieceSquare(board, board.Turn());
    const auto nus = EvalPieceSquare(board, !board.Turn());
    return {us.first - nus.first, us.second - nus.second};
}
} // namespace

int Eval(const Board &board) {
    const std::pair<int, int> game_phase = CalculatePhase(board);

    int value = 0;

    const std::pair<int, int> piece_square = EvalPieceSquare(board);
    value += game_phase.first * piece_square.first;
    value += game_phase.second * piece_square.second;

    return value / 24;
}
