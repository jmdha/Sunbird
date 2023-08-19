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
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(pos, pos.GetTurn());
    if (moves.empty())
        return Evaluation::Eval(pos) == 0 ? AlternativeResult::Draw : AlternativeResult::Checkmate;
    else
        return {};
}
} // namespace

std::variant<Move, AlternativeResult> GetBestMove(Board &board, int depth) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();

    PV pv = PV(board.Ply());
    Internal::Negamax(board, -Values::INF, Values::INF, depth, pv, pv);
    return pv._moves[0];
}

std::variant<Move, AlternativeResult> GetBestMoveTime(Board &board, std::optional<int> timeLimit) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();
    if (auto moves =
            MoveGen::GenerateMoves<MoveGen::GenType::All>(board.Pos(), board.Pos().GetTurn());
        moves.size() == 1)
        return moves[0];

    std::cout << "info fen " << Export::FEN(board.Pos()) << '\n';

    PV pv = PV(board.Ply());

    std::jmp_buf exitBuffer;
    SearchLimit *limit = nullptr;
    if (timeLimit.has_value())
        limit = new SearchLimit(exitBuffer, timeLimit.value());

    if (setjmp(exitBuffer)) {
        free(limit);
        return pv._moves[0];
    }

    for (int depth = 1; depth < 1000; depth++) {
        Board tempBoard = board;
        auto t0 = std::chrono::steady_clock::now();
        PV tempPV;
        int score = -Internal::Negamax(tempBoard, -Values::INF, Values::INF, depth,
                                       tempPV, pv, limit);
        // HACK: This fixes a bug where sometimes checkmates in high depths would return no pv. It
        // should not be needed, but I cannot find why this occurs
        if (tempPV._count == 0)
            break;
        pv._count = tempPV._count;
        pv._moves = tempPV._moves;
        auto t1 = std::chrono::steady_clock::now();
        size_t t = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        std::cout << "info";
        printf(" depth %2d", depth);
        printf(" score cp %6d", score);
        printf(" time %5zu ms", t);
        auto nodes = tempBoard.MoveCount() - board.MoveCount();
        printf(" nodes %9zu", nodes);
        auto nps = nodes / std::max((size_t)1, (t / 1000));
        printf(" nps %8zu", nps);
        std::cout << " pv ";
        for (int i = 0; i < std::min(pv._count, 8); i++)
            std::cout << pv._moves[i].ToString() << " ";
        std::cout << '\n';
        if (std::abs(score) == Values::INF)
            break;
    }
    longjmp(exitBuffer, 1);
}
} // namespace Chess::Engine::Search
