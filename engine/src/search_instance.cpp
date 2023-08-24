#include "chess/internal/move_list.hpp"
#include "engine/evaluation.hpp"
#include "engine/internal/move_ordering.hpp"
#include "engine/internal/values.hpp"
#include <chess/move_gen.hpp>
#include <chrono>
#include <csetjmp>
#include <cstdlib>
#include <cstring>
#include <engine/internal/search_instance.hpp>

namespace Chess::Engine::Search {
using namespace MoveGen;
Instance::Instance(int depth) : _rootDepth(depth), _priorPV({}){};
Instance::Instance(int depth, int time, std::optional<Result> prior)
    : _rootDepth(depth),
      _time(std::chrono::steady_clock::now() + std::chrono::milliseconds(time)),
      _priorPV(prior.has_value() ? prior->_pv : PV(0)) {}

namespace {
bool AB(int score, int &alpha, int beta) {
    if (score >= beta)
        return true;
    if (score > alpha)
        alpha = score;
    return false;
}
} // namespace

bool Instance::IsTime() {
    return _time < std::chrono::steady_clock::now();
}

int Instance::Quiesce(Board &board, int alpha, int beta) {
    int standPat = Evaluation::Eval(board.Pos());
    if (AB(standPat, alpha, beta))
        return beta;

    MoveList moves = GenerateMoves<GenType::Attack>(board.Pos());
    MoveOrdering::All(board, _priorPV, moves);
    for (auto move : moves) {
        board.MakeMove(move);
        int score = -Quiesce(board, -beta, -alpha);
        board.UndoMove();
        if (AB(score, alpha, beta))
            return beta;
    }

    return alpha;
}
int Instance::Negamax(Board &board, int alpha, int beta, int depth, PV &pv,
                      std::jmp_buf *jmpBuf) {
    if (jmpBuf != nullptr && IsTime())
        longjmp(*jmpBuf, 1);
    if (board.IsThreefoldRepetition())
        return 0;
    if (depth == 0)
        return Quiesce(board, alpha, beta);

    bool anyMove = false;

    for (auto moves : {GenerateMoves<GenType::Attack>(board.Pos()),
                       GenerateMoves<GenType::Quiet>(board.Pos())}) {
        MoveOrdering::All(board, _priorPV, moves);
        for (auto move : moves) {
            anyMove = true;
            PV moveLine;
            board.MakeMove(move);
            int score =
                -Negamax(board, -beta, -alpha, depth - 1, moveLine, jmpBuf);
            board.UndoMove();
            if (alpha >= beta)
                return beta;
            if (score > alpha) {
                alpha = score;
                pv._moves[0] = move;
                std::memmove(&pv._moves[1], &moveLine._moves[0],
                             moveLine._count * sizeof(Move));
                pv._count = moveLine._count + 1;
            }
        }
    }

    if (!anyMove)
        return Evaluation::EvalNoMove(board.Pos());

    return alpha;
}
Instance::Result Instance::Begin(Board &board, std::jmp_buf *jmpBuf) {
    PV pv = PV(board.Ply());
    int score =
        Negamax(board, -Values::INF, Values::INF, _rootDepth, pv, jmpBuf);
    return Result{._pv = pv, ._move = pv._moves[0], ._score = score};
}

} // namespace Chess::Engine::Search
