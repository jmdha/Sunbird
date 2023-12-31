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
#include <engine/internal/tt.hpp>
#include <engine/internal/values.hpp>
#include <engine/search.hpp>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <utility>

namespace Chess::Engine::Search {
namespace {
std::optional<AlternativeResult> IsTerminal(const Position &pos) {
    const MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(pos, pos.GetTurn());
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

    PV pv = PV();
    int alpha = -Values::INF;
    int beta = Values::INF;
    const size_t priorMoveCount = board.MoveCount();
    for (int depth = 1; depth < 256 && !setjmp(exitBuffer); depth++) {
        auto t0 = std::chrono::steady_clock::now();
        int score = Internal::Negamax(board, alpha, beta, depth, 0, pv, &limit);
        if ((score <= alpha) || (score >= beta)) {
            alpha = -Values::INF;
            beta = Values::INF;
            score = Internal::Negamax(board, alpha, beta, depth, 0, pv, &limit);
        } else {
            alpha = score - 25;
            beta = score + 25;
        }
        auto t1 = std::chrono::steady_clock::now();
        size_t t = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        timeLimit -= t;
        pv = ExtractPV(board);
        std::cout << "info";
        printf(" depth %d", depth);
        printf(" score cp %d", score);
        printf(" time %zu ms", t);
        auto nodes = board.MoveCount() - priorMoveCount;
        printf(" nodes %zu", nodes);
        auto nps = nodes / (double)t * 1000;
        printf(" nps %zu", (size_t)nps);
        printf(" hashfull %zu", TT::HashFull());
        std::cout << " pv ";
        for (int i = 0; i < pv.size(); i++)
            std::cout << pv[i].ToString() << " ";
        std::cout << '\n';
        if (std::abs(score) == Values::INF)
            break;
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
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();
    assert(depth > 0);

    std::optional<std::pair<Move, int>> bestMove;
    for (auto move : MoveGen::GenerateMoves(board.Pos())) {
        board.MakeMove(move);
        int value = -Internal::Negamax(board, -Values::INF, Values::INF, depth - 1, 0, PV());
        board.UndoMove();
        if (!bestMove.has_value() || value > bestMove.value().second)
            bestMove = {move, value};
    }

    return bestMove.value().first;
}

std::variant<Move, AlternativeResult> GetBestMoveTime(Board &board, int timeLimit) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();
    if (auto moves = MoveGen::GenerateMoves(board.Pos()); moves.size() == 1)
        return moves[0];

    std::cout << "info fen " << Export::FEN(board.Pos()) << '\n';

    return IterativeDeepening(board, timeLimit);
}
} // namespace Chess::Engine::Search
