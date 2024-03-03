#include "search.hpp"
#include "evaluation.hpp"
#include "move_gen.hpp"
#include "move_ordering.hpp"
#include "pv.hpp"
#include "tt.hpp"
#include "types.hpp"
#include "values.hpp"
#include <csetjmp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <utility>

namespace Search {
namespace {
std::optional<AlternativeResult> IsTerminal(const Position &pos) {
    const MoveList moves = MoveGen::GenerateAll(pos, pos.GetTurn());
    if (moves.empty())
        return Evaluation::Eval(pos) == 0 ? AlternativeResult::Draw : AlternativeResult::Checkmate;
    else
        return {};
}

PV ExtractPV(Board board) {
    const size_t ply = board.Ply();
    std::vector<Move> moves;
    while (moves.size() < 8) {
        const Move move = TT::ProbeMove(board.Pos().GetHash());
        if (move.GetValue() != 0) {
            moves.push_back(move);
            board.MakeMove(move);
        } else {
            break;
        }
    }
    return PV(ply, moves);
}

std::variant<Move, AlternativeResult> IterativeDeepening(Board &board, int timeLimit) {
    std::jmp_buf exitBuffer;
    SearchLimit limit = SearchLimit(exitBuffer, timeLimit);

    PV pv                       = PV();
    int alpha                   = -Values::INF;
    int beta                    = Values::INF;
    const size_t priorMoveCount = board.MoveCount();
    for (int depth = 1; depth < MAX_SEARCH_DEPTH && !setjmp(exitBuffer); depth++) {
        auto t0   = std::chrono::steady_clock::now();
        int score = Internal::Negamax(board, alpha, beta, depth, 0, pv, &limit);
        if ((score <= alpha) || (score >= beta)) {
            alpha = -Values::INF;
            beta  = Values::INF;
            score = Internal::Negamax(board, alpha, beta, depth, 0, pv, &limit);
        } else {
            alpha = score - 50;
            beta  = score + 50;
        }
        auto t1  = std::chrono::steady_clock::now();
        size_t t = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        timeLimit -= t;
        pv                 = ExtractPV(board);
        const size_t nodes = board.MoveCount() - priorMoveCount;
        printf(
            "info depth %d score cp %d time %zu ms nodes %zu nps %zu hashfull %zu pv", depth, score,
            t, nodes, nodes * 1000 / std::max(t, (size_t)1), TT::HashFull()
        );
        for (size_t i = 0; i < pv.size(); i++)
            std::cout << pv[i].ToString() << " ";
        std::cout << '\n';
        std::flush(std::cout);
        if (std::abs(score) == Values::INF) break;
    }

    if (pv.empty()) {
        // This should not happen, but nevertheless it sometimes does
        printf("info pv empty, doing manual search at depth 3\n");
        TT::Clear();
        return GetBestMoveDepth(board, 3);
    } else {
        return pv[0];
    }
}
} // namespace

std::variant<Move, AlternativeResult> GetBestMoveDepth(Board &board, int depth) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value()) return terminal.value();
    assert(depth > 0);

    std::optional<std::pair<Move, int>> bestMove;
    for (auto move : MoveGen::GenerateAll(board.Pos(), board.Pos().GetTurn())) {
        board.MakeMove(move);
        int value = -Internal::Negamax(board, -Values::INF, Values::INF, depth - 1, 0, PV());
        board.UndoMove();
        if (!bestMove.has_value() || value > bestMove.value().second) bestMove = {move, value};
    }

    return bestMove.value().first;
}

std::variant<Move, AlternativeResult> GetBestMoveTime(Board &board, int timeLimit) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value()) return terminal.value();
    if (auto moves = MoveGen::GenerateAll(board.Pos(), board.Pos().GetTurn()); moves.size() == 1)
        return moves[0];

    return IterativeDeepening(board, timeLimit);
}
} // namespace Search
