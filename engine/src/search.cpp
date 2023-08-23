#include "engine/internal/search_instance.hpp"
#include <chess/export.hpp>
#include <chess/internal/constants.hpp>
#include <chess/move_gen.hpp>
#include <chrono>
#include <csetjmp>
#include <cstdlib>
#include <cstring>
#include <engine/evaluation.hpp>
#include <engine/internal/move_ordering.hpp>
#include <engine/internal/pv.hpp>
#include <engine/internal/values.hpp>
#include <engine/search.hpp>
#include <iostream>
#include <optional>

namespace Chess::Engine::Search {
namespace {
std::optional<AlternativeResult> IsTerminal(const Position &pos) {
    MoveList moves = MoveGen::GenerateMoves(pos);
    if (moves.empty())
        return Evaluation::Eval(pos) == 0 ? AlternativeResult::Draw
                                          : AlternativeResult::Checkmate;
    else
        return {};
}
} // namespace

std::variant<Move, AlternativeResult> GetBestMove(Board &board, int depth) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();

    auto result = Instance(depth).Begin(board);
    return result._move;
}

std::variant<Move, AlternativeResult>
GetBestMoveTime(Board &board, std::optional<int> timeLimit) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();
    if (auto moves = MoveGen::GenerateMoves(board.Pos()); moves.size() == 1)
        return moves[0];

    std::cout << "info fen " << Export::FEN(board.Pos()) << '\n';

    std::jmp_buf exitBuffer;
    std::optional<Instance::Result> prior;

    for (int depth = 1; depth < 1000 && !setjmp(exitBuffer); depth++) {
        Board tempBoard = board;
        auto t0 = std::chrono::steady_clock::now();
        auto result =
            Instance(depth, timeLimit, prior).Begin(tempBoard, &exitBuffer);
        auto t1 = std::chrono::steady_clock::now();
        // HACK: This fixes a bug where sometimes checkmates in high depths
        // would return no pv. It should not be needed, but I cannot find why
        // this occurs
        if (result._pv._count == 0)
            break;
        prior = result;
        size_t t =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)
                .count();
        std::cout << "info";
        printf(" depth %d", depth);
        printf(" score cp %d", result._score);
        printf(" time %zu ms", t);
        auto nodes = tempBoard.MoveCount() - board.MoveCount();
        printf(" nodes %zu", nodes);
        auto nps = nodes / std::max((size_t)1, t) * 1000;
        printf(" nps %zu", nps);
        std::cout << " pv ";
        for (int i = 0; i < std::min(result._pv._count, 8); i++)
            std::cout << result._pv._moves[i].ToString() << " ";
        std::cout << '\n';
        if (std::abs(result._score) == Values::INF)
            break;
    }
    if (prior.has_value())
        return prior.value()._move;
    else
        return std::get<Move>(GetBestMove(board, 1));
}
} // namespace Chess::Engine::Search
